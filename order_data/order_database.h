#include "iostream"
#include <winSock.h>
#include <mysql.h>
#include <stdio.h>


#include <Windows.h>
#include <string>
#include<stdlib.h>
#include<fstream>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<time.h>
#include"order_data.h"

#define OPENSSLKEY "..\\..\\key\\private_key.pem"
#define PUBLICKEY "..\\..\\key\\public_key.pem"
#define BUFFSIZE 1024
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


//using order_proto::OrderInformation;
using namespace std;
class OrderDataBase 
{
private:
	
	MYSQL mysql;
	char dataBase_userName[255];   //数据库用户名username
	char dataBase_password[255];    //数据库密码，填自己的密码
	char dataBase_host[255];   //数据库连接地址，注意有时候使用主机ip会报错，此时可以将ip改为localhost
	char dataBase_name[255];
	unsigned int dataBase_port;
	bool my_encrypt(const char* str, const char* path_key, string& sign_string);
	bool dsaVerify(const string& sign_string, const string& verify_string, const char* path_key);
	void parseOrderDataToString(const OrderData* data,string& result);

public:
	OrderDataBase(const char* userName = "root", const char* password = "123456", const char* host = "localhost", const char* name = "order_system", unsigned int port = 3306);
	bool findOrder(int merchant_id, string merchant_order_id, OrderData* reply);
	bool addOrder(const OrderData* request);
	bool connectDataBase(); //连接数据库
	void freeConnect();   //关闭数据库
	
	};

