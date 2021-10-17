#include"http_service.h"

extern string grpcServerIp = "127.0.0.1";
extern int grpcServerPort = 50051;

string dump_headers(const Headers& headers)
{
	string s;
	char buf[BUFSIZ];

	for (auto it = headers.begin(); it != headers.end(); ++it) {
		const auto& x = *it;
		snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
		s += buf;
	}

	return s;
}
string log(const Request& req, const Response& res)
{

	string s;
	char buf[BUFSIZ];

	s += "================================\n";

	snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
		req.version.c_str(), req.path.c_str());
	s += buf;

	string query;
	for (auto it = req.params.begin(); it != req.params.end(); ++it) {
		const auto& x = *it;
		snprintf(buf, sizeof(buf), "%c%s=%s",
			(it == req.params.begin()) ? '?' : '&', x.first.c_str(),
			x.second.c_str());
		query += buf;
	}
	snprintf(buf, sizeof(buf), "%s\n", query.c_str());
	s += buf;

	s += dump_headers(req.headers);

	s += "--------------------------------\n";

	snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
	s += buf;
	s += dump_headers(res.headers);
	s += "\n";

	if (!res.body.empty()) { s += res.body; }

	s += "\n";

	return s;
}

int http_initial(Server& httpSvr,string serverIp,int port)
{
	grpcServerIp = serverIp;
	grpcServerPort = port;

	httpSvr.set_error_handler([](const Request& /*req*/, Response& res) {
		string result = "Error Status: " + to_string(res.status) + "\n" + res.body;
		res.set_content(result, "text/plain");
		});

	httpSvr.set_logger([](const Request& req, const Response& res) {
		printf("%s", log(req, res).c_str());
		});

	/*httpSvr.Get("/order", http_add_order);*/
	/*httpSvr.Post("/order", http_query_order);*/
	httpSvr.Get("/order", [](const Request& req, Response& rsp) {
		//check param of Request
		if ((!req.has_param("merchant_id")) || (!req.has_param("merchant_order_id")))
		{
			rsp.body = "There are no enough parameters!";
			rsp.status = 404;
			return;
		}

		string merchant_id = req.get_param_value("merchant_id");
		string merchant_order_id = req.get_param_value("merchant_order_id");
		if (!isNumber(merchant_id) || (!isNumber(merchant_order_id)))
		{
			rsp.body = "Parameters are not in correct format!";
			rsp.status = 404;
			return;
		}

		// grpc client
		string serverAddress = grpcServerIp +":"+ to_string(grpcServerPort);

		OrderClient order(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));
		OrderInformation reply;
		order.QueryOrder(atoi(merchant_id.c_str()), atoi(merchant_order_id.c_str()), &reply);


		//response
		if (reply.description() == "")
		{
			rsp.body = "Query the order failed!";
			rsp.status = 404;
			return;
		}
		else
		{
			std::stringstream ss;
			ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << reply.amount();
			string amount = ss.str();

			Json::Value rootValue = Json::objectValue;
			rootValue["merchant_id"] = reply.merchant_id();
			rootValue["merchant_order_id"] = reply.merchant_order_id();
			rootValue["description"] = reply.description();
			rootValue["address"] = reply.address();
			rootValue["amount"] = amount;
			rootValue["currency"] = reply.currency();

			std::ostringstream os;
			Json::StreamWriterBuilder writerBuilder;
			std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
			jsonWriter->write(rootValue, &os);
			rsp.body = os.str();

			rsp.status = 200;
			return;
		}
		});

	httpSvr.Post("/order", [](const Request& req, Response& rsp) {
		//check param of Request
		string body = req.body;
		cout << "body: " << body << endl;
		if (body.empty())
		{
			rsp.body = "There are no enough parameters!";
			rsp.status = 404;
			return;
		}


		bool res;

		Json::Value rootValue = split(body.c_str());

		if ((!rootValue.isMember("merchant_id")) || (!rootValue.isMember("merchant_order_id")) || (!rootValue.isMember("description")) || (!rootValue.isMember("address")) || (!rootValue.isMember("amount")) || (!rootValue.isMember("currency")))
		{
			rsp.body = "There are no enough parameters!";
			rsp.status = 404;
			return;
		}

		if (!isNumber(rootValue["merchant_id"].asString()) || !isNumber(rootValue["merchant_order_id"].asString())|| (!isDouble(rootValue["amount"].asString())))
		{
			rsp.body = "Parameters are not in correct format!";
			rsp.status = 404;
			return;
		}
		

		if (!isUrl(rootValue["address"].asString())) 
		{
			rsp.body = "Parameters are not in correct format!";
			rsp.status = 404;
			return;
		}

		int merchant_id = stoi(rootValue["merchant_id"].asString());
		int merchant_order_id = stoi(rootValue["merchant_order_id"].asString());
		string description = rootValue["description"].asString();
		string address = rootValue["address"].asString();
		double amount = stod(rootValue["amount"].asString());
		string currency = rootValue["currency"].asString();



		//grpc client
		string serverAddress = grpcServerIp + ":" + to_string(grpcServerPort);
		OrderClient order(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));
		OrderData data1(merchant_id, merchant_order_id, description, address, amount, currency);
		string result = order.AddOrder(&data1);
		rsp.body = result;
		rsp.status = 404;
		if (result == "Add the order success!")
		{
			rsp.status = 200;
		}
		});

	return 0;
}
