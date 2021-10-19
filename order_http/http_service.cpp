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
	
	httpSvr.Get(R"(/v3/pay/transactions/out-trade-no/(([-\w\*]){6,32}))", [](const Request& req, Response& rsp) {
		//check param of Request
		if (req.has_param("timestamp")) 
		{
			time_t now = time(NULL);
			int pre =stoi(req.get_param_value("timestamp"));
			if (now - pre > 3000) 
			{
				rsp.body = "Request Timeout";
				rsp.status = 408;
				return;
			}
		}

		if (!req.has_param("mchid"))
		{
			rsp.body = "Please check if the mchid is correct";
			rsp.status = 400;
			return;
		}

		string merchant_id = req.get_param_value("mchid");
		string merchant_order_id = req.matches[1];
		cout << "merchant_id: " << merchant_id << endl;
		cout << "merchant_order_id: " << merchant_order_id << endl;
		if (!isNumber(merchant_id))
		{
			rsp.body = "Please check if the mchid is correct";
			rsp.status = 400;
			return;
		}

		// grpc client
		string serverAddress = grpcServerIp +":"+ to_string(grpcServerPort);

		OrderClient order(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));
		OrderInformation reply;
		order.QueryOrder(atoi(merchant_id.c_str()), merchant_order_id, &reply);


		//response
		if (reply.description() == "")
		{
			rsp.body = "Please check whether the order has initiated a transaction";
			rsp.status = 404;
			return;
		}
		else
		{
			std::stringstream ss;
			ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << reply.amount();
			string amount = ss.str();

			Json::Value rootValue = Json::objectValue;
			Json::Value amountObject = Json::objectValue;
			rootValue["mchid"] = reply.merchant_id();
			rootValue["out_trade_no"] = reply.merchant_order_id();
			rootValue["description"] = reply.description();
			rootValue["notify_url"] = reply.address();
			amountObject["total"] = amount;
			amountObject["currency"] = reply.currency();
			rootValue["amount"] = amountObject;


			std::ostringstream os;
			Json::StreamWriterBuilder writerBuilder;
			std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
			jsonWriter->write(rootValue, &os);
			rsp.body = os.str();

			rsp.status = 200;
			return;
		}
		});

	httpSvr.Post("/v3/pay/transactions/jsapi", [](const Request& req, Response& rsp) {
		//check param of Request
		rsp.status = 200;
		Json::Value rootValue = parseBody(req, rsp);
		if (rsp.status != 200) 
		{
			return;
		}
		
		int merchant_id = rootValue["mchid"].asInt();
		string merchant_order_id = rootValue["out_trade_no"].asString();
		string description = rootValue["description"].asString();
		string address = rootValue["notify_url"].asString();
		double amount = rootValue["amount"]["total"].asDouble();
		string currency = rootValue["amount"]["currency"].asString();

		if (rootValue.isMember("timestamp"))
		{
			time_t now = time(NULL);
			int pre = rootValue["timestamp"].asInt64();
			
			if (now - pre > 3000)
			{
				rsp.body = "Request Timeout";
				rsp.status = 408;
				return;
			}
		}

		//grpc client
		string serverAddress = grpcServerIp + ":" + to_string(grpcServerPort);
		OrderClient order(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials()));
		OrderData data1(merchant_id, merchant_order_id, description, address, amount, currency);
		string result = order.AddOrder(&data1);
		rsp.body = result;
		rsp.status = 200;
		if (result != "Add the order success!")
		{
			rsp.body = "Please verify whether the merchant order number has been submitted repeatedly";
			rsp.status = 403;
		}
		
		});

	return 0;
}

Json::Value parseBody(const Request& req, Response& rsp)
{
	string body = req.body;
	cout << endl << "body: " << body << endl;
	bool res;
	JSONCPP_STRING errs;
	Json::Value rootValue= Json::objectValue;
	Json::Value amountObject = Json::objectValue;
	Json::CharReaderBuilder readerBuilder;

	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
	res = jsonReader->parse(body.c_str(), body.c_str() + body.length(), &rootValue, &errs);
	if (!res || !errs.empty()) {
		cout << 1 << endl;
		rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
		rsp.status = 404;
		return rootValue;
	}

	if ((!rootValue.isMember("mchid")) || (!rootValue.isMember("out_trade_no")) || (!rootValue.isMember("description")) || (!rootValue.isMember("notify_url")) || (!rootValue.isMember("amount")))
	{
		cout << 2 << endl;
		rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
		rsp.status = 404;
		return rootValue;
	}

	//string amountObjectStr = rootValue["amount"].asString();
	//res = jsonReader->parse(amountObjectStr.c_str(), amountObjectStr.c_str() + amountObjectStr.length(), &amountObject, &errs);
	//if (!res || !errs.empty()) {
	//	rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
	//	rsp.status = 404;
	//	return rootValue;
	//}

	amountObject = rootValue["amount"];
	if ((!amountObject.isMember("total")) || (!amountObject.isMember("currency")))
	{
		cout << 3 << endl;
		rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
		rsp.status = 404;
		return rootValue;
	}


	if ((!rootValue["mchid"].isInt()) || (!amountObject["total"].isDouble()))
	{
		cout << 4 << endl;
		rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
		rsp.status = 404;
		return rootValue;
	}

	if (!isUrl(rootValue["notify_url"].asString()))
	{
		cout << 5 << endl;
		rsp.body = "Please check the request parameters according to the detailed information returned by the interface ";
		rsp.status = 404;
		return rootValue;
	}

	return rootValue;
}