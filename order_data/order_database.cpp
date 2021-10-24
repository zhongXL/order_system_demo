#ifdef __cplusplus
extern "C" {
#include <applink.c>
}
#endif
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
bool OrderDataBase::findOrder(int merchant_id, string merchant_order_id, OrderData* reply) 
{
	bool result = false;
	MYSQL_RES* res;
	MYSQL_ROW row;
	
	string query = "SELECT merchant_id,merchant_order_id,description,address,amount,currency,UNHEX(sign) from `order` Where merchant_id = " + to_string(merchant_id) + " AND merchant_order_id = \"" + merchant_order_id+"\"";
	
	mysql_query(&mysql, query.c_str());
	
	res = mysql_store_result(&mysql);
	
	row = mysql_fetch_row(res);
	
	if (row)
	{
		reply->set_merchant_id(atoi(row[0]));
		reply->set_merchant_order_id(row[1]);
		reply->set_description(row[2]);
		reply->set_address(row[3]);
		reply->set_amount(strtod(row[4], NULL));
		reply->set_currency(row[5]);
		string sign_string = row[6];
		
		string verify_string;
		parseOrderDataToString(reply, verify_string);
		
		if (dsaVerify(sign_string, verify_string, PUBLICKEY))
		{
			result = true;
		}
	}

	return result;
}

bool OrderDataBase::addOrder(const OrderData* request)
{
	
	bool result = false;
	OrderData findReply;
	
	result = findOrder(request->get_merchant_id(), request->get_merchant_order_id(), &findReply);
	
	if (result)
	{
		return findReply.isSame(request);
	}

	else
	{
		char* merchant_order_id;
		char* description;
		char* address;
		char* currency;
		merchant_order_id = (char*)malloc(sizeof(char) * (request->get_merchant_order_id()).length() * 3);
		memset(merchant_order_id, 0, (request->get_merchant_order_id()).length() * 3);
		description = (char*)malloc(sizeof(char) * (request->get_description()).length() * 3);
		memset(description, 0, (request->get_description()).length() * 3);
		address = (char*)malloc(sizeof(char) * (request->get_address()).length() * 3);
		memset(address, 0, (request->get_address()).length() * 3);
		currency = (char*)malloc(sizeof(char) * (request->get_currency()).length() * 3);
		memset(currency, 0, (request->get_currency()).length() * 3);
		mysql_real_escape_string(&mysql, merchant_order_id, (request->get_merchant_order_id()).c_str(), (request->get_merchant_order_id()).length());
		mysql_real_escape_string(&mysql, description, (request->get_description()).c_str(), (request->get_description()).length());
		mysql_real_escape_string(&mysql, address, (request->get_address()).c_str(), (request->get_address()).length());
		mysql_real_escape_string(&mysql, currency, (request->get_currency()).c_str(), (request->get_currency()).length());
		

		string input_string;
		string sign_string;
		parseOrderDataToString(request, input_string);
		bool encrypt_result=my_encrypt(input_string.c_str(), PUBLICKEY, sign_string);
		
		if (!encrypt_result) 
		{
			return false;
		}
		
		
		

		/*string query = "INSERT INTO `order` (merchant_id,merchant_order_id,description,address,amount,currency) VALUES ("+to_string(request->get_merchant_id())+","+to_string(request->get_merchant_order_id())+",'"+request->get_description()+"','"+request->get_address()+"',"+to_string(request->get_amount())+",'"+request->get_currency()+"')";
		*/
		
		string query = "INSERT INTO `order` (merchant_id,merchant_order_id,description,address,amount,currency,sign) VALUES (" + to_string(request->get_merchant_id()) + ",'" + request->get_merchant_order_id() + "','" + description + "','" + address + "'," + to_string(request->get_amount()) + ",'" + currency + "',HEX('"+ sign_string +"'))";

		bool flag = mysql_query(&mysql, query.c_str());
		
		if (flag == 0)
		{
			result = true;
			
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

void OrderDataBase::parseOrderDataToString(const OrderData* data,string & result)
{
	result = "merchant_id="+to_string(data->get_merchant_id())+"&merchant_order_id="+data->get_merchant_order_id();
	result = result + "&description=" + data->get_description() + "&address=" + data->get_address();
	result = result + "&amount=" + to_string(data->get_amount()) + "&currency=" + data->get_currency();
	return ;
}

bool OrderDataBase::my_encrypt(const char* str,const char* path_key,string& sign_string) {
	char* p_en;
	RSA* p_rsa;
	FILE* file;
	int flen, rsa_len;
	if ((file = fopen(path_key, "r")) == NULL) {
		perror("open key file error");
		return false;
	}
	if ((p_rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL) {
		//if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){   换成这句死活通不过，无论是否将公钥分离源文件
		ERR_print_errors_fp(stdout);
		return false;
	}
	flen = strlen(str);
	rsa_len = RSA_size(p_rsa);
	p_en = (char*)malloc(rsa_len + 1);
	memset(p_en, 0, rsa_len + 1);
	if (RSA_public_encrypt(rsa_len, (unsigned char*)str, (unsigned char*)p_en, p_rsa, RSA_NO_PADDING) < 0) {
		return NULL;
	}
	RSA_free(p_rsa);
	fclose(file);
	sign_string = p_en;
	return true;
}

bool OrderDataBase::dsaVerify(const string& sign_string, const string& verify_string,const char* path_key)
{
	string rsa_string = "";
	my_encrypt(verify_string.c_str(), path_key,rsa_string);
	return sign_string == rsa_string;
}


