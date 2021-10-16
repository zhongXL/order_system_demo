#include"order_database.h"
using namespace std;
OrderDataBase::OrderDataBase(const char* userName, const char* password , const char* host , const char* name , unsigned int port)
{
	memset(dataBase_userName, 0, 255);
	memcpy(dataBase_userName, userName, strlen(userName));

	memset(dataBase_password, 0, 255);
	memcpy(dataBase_password, password, strlen(password));

	memset(dataBase_host, 0, 255);
	memcpy(dataBase_host, host, strlen(host));

	memset(dataBase_name, 0, 255);
	memcpy(dataBase_name, name, strlen(name));
	
	dataBase_port = port;
}
int OrderDataBase::findOrder(int merchant_id, int merchant_order_id, OrderData* reply) 
{
	
	int result = 0;
	MYSQL_RES* res;
	MYSQL_ROW row;
	
	string query = "SELECT * from `order` Where merchant_id = " + to_string(merchant_id) + " AND merchant_order_id = " + to_string(merchant_order_id);
	mysql_query(&mysql, query.c_str());
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row)
	{
		reply->set_merchant_id(atoi(row[0]));
		reply->set_merchant_order_id(atoi(row[1]));
		reply->set_description(row[2]);
		reply->set_address(row[3]);
		reply->set_amount(strtod(row[4], NULL));
		reply->set_currency(row[5]);
		result = 1;
	}
	return result;
}

int OrderDataBase::addOrder(const OrderData* request)
{
	
	int result = 0;
	OrderData findReply;
	result = findOrder(request->get_merchant_id(), request->get_merchant_order_id(), &findReply);
	if (result==1)
	{
		if ((request->get_merchant_id() == findReply.get_merchant_id()) && (request->get_merchant_order_id() == findReply.get_merchant_order_id()) && (request->get_description() == findReply.get_description()) && (request->get_address() == findReply.get_address()) && (request->get_amount() == findReply.get_amount()) && (request->get_currency() == findReply.get_currency()))
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	}

	else
	{
		char* description;
		char* address;
		char* currency;
		description = (char*)malloc(sizeof(char) * (request->get_description()).length() * 3);
		memset(description, 0, (request->get_description()).length() * 3);
		address = (char*)malloc(sizeof(char) * (request->get_address()).length() * 3);
		memset(address, 0, (request->get_address()).length() * 3);
		currency = (char*)malloc(sizeof(char) * (request->get_currency()).length() * 3);
		memset(currency, 0, (request->get_currency()).length() * 3);
		mysql_real_escape_string(&mysql, description, (request->get_description()).c_str(), (request->get_description()).length());
		mysql_real_escape_string(&mysql, address, (request->get_address()).c_str(), (request->get_address()).length());
		mysql_real_escape_string(&mysql, currency, (request->get_currency()).c_str(), (request->get_currency()).length());

		/*string query = "INSERT INTO `order` (merchant_id,merchant_order_id,description,address,amount,currency) VALUES ("+to_string(request->get_merchant_id())+","+to_string(request->get_merchant_order_id())+",'"+request->get_description()+"','"+request->get_address()+"',"+to_string(request->get_amount())+",'"+request->get_currency()+"')";
		*/
		string query = "INSERT INTO `order` (merchant_id,merchant_order_id,description,address,amount,currency) VALUES (" + to_string(request->get_merchant_id()) + "," + to_string(request->get_merchant_order_id()) + ",'" + description + "','" + address + "'," + to_string(request->get_amount()) + ",'" + currency + "')";

		bool flag = mysql_query(&mysql, query.c_str());
		if (flag == 0)
		{
			result = 1;
		}

	}
	return result;
}

bool OrderDataBase::connectDataBase()
{
	//初始化mysql
	mysql_init(&mysql);
	if (!(mysql_real_connect(&mysql, dataBase_host, dataBase_userName, dataBase_password, dataBase_name, dataBase_port, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
		{
			printf("Error connecting to database:%s\n", mysql_error(&mysql));
			return false;
		}
		else
		{
			cout << "Correct connecting to database!" << endl << "Connecting..." << endl;
			return true;
		}
}

void OrderDataBase::freeConnect()
{
	//关闭数据库连接
	mysql_close(&mysql);
}

//int main(int argc, char** argv) {
//	// Instantiate the client. It requires a channel, out of which the actual RPCs
//	// are created. This channel models a connection to an endpoint (in this case,
//	// localhost at port 50051). We indicate that the channel isn't authenticated
//	// (use of InsecureChannelCredentials()).
//
//	int merchantID = 0;
//	int merchantOrderID = 4;
//	string description = "client test";
//	/*string address = "https://www.baidu.com/client/";*/
//	string address = "https"; 
//	double amount = 3;
//	string currency = "RMB";
//	OrderData data1(merchantID, merchantOrderID, description, address, amount, currency);
//	OrderDataBase order;
//	int result = order.addOrder(&data1);
//	cout << "Add Order Result:\n" << result << endl;
//
//	/*data1.set_amount(3);
//	result = order.addOrder(&data1);
//	cout << "Add Order Result:\n" << result << endl;
//
//	data1.set_merchant_id(3);
//	result = order.addOrder(&data1);
//	cout << "Add Order Result:\n" << result << endl;*/
//
//	OrderData reply;
//	
//	result=order.findOrder(merchantID, merchantOrderID, &reply);
//	if (reply.get_description() == "")
//	{
//		cout << "Query the order failed!" << endl;
//	}
//	else
//	{
//		cout << "merchant ID: " << reply.get_merchant_id() << endl;
//		cout << "merchant order ID: " << reply.get_merchant_order_id() << endl;
//		cout << "description: " << reply.get_description() << endl;
//		cout << "address: " << reply.get_address() << endl;
//		cout << "amount: " << reply.get_amount() << endl;
//		cout << "currency: " << reply.get_currency() << endl;
//	}
//
//	//OrderData reply1;
//	//order.findOrder(1, 0, &reply1);
//	//if (reply1.get_description() == "")
//	//{
//	//	cout << "Query the order failed!" << endl;
//	//}
//	//else
//	//{
//	//	cout << "merchant ID: " << reply1.get_merchant_id() << endl;
//	//	cout << "merchant order ID: " << reply1.get_merchant_order_id() << endl;
//	//	cout << "description: " << reply1.get_description() << endl;
//	//	cout << "address: " << reply1.get_address() << endl;
//	//	cout << "amount: " << reply1.get_amount() << endl;
//	//	cout << "currency: " << reply1.get_currency() << endl;
//	//}
//
//	system("pause");
//	return 0;
//}
