#include "iostream"
#include <winSock.h>
#include <mysql.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include<stdlib.h>
#include"order_data.h"


//using order_proto::OrderInformation;
using namespace std;
class OrderDataBase 
{
private:
	
	MYSQL mysql;
	char dataBase_userName[255];   //���ݿ��û���username
	char dataBase_password[255];    //���ݿ����룬���Լ�������
	char dataBase_host[255];   //���ݿ����ӵ�ַ��ע����ʱ��ʹ������ip�ᱨ����ʱ���Խ�ip��Ϊlocalhost
	char dataBase_name[255];
	unsigned int dataBase_port;
	
	

public:
	OrderDataBase(const char* userName = "root", const char* password = "123456", const char* host = "localhost", const char* name = "order_system", unsigned int port = 3306);
	int findOrder(int merchant_id, int merchant_order_id, OrderData* reply);
	int addOrder(const OrderData* request);
	bool connectDataBase(); //�������ݿ�
	void freeConnect();   //�ر����ݿ�
	
	
};

