#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H
#endif
#include <chrono>
#include <cstdio>
#include<iostream>
#include<string>
#include<json/json.h>
#include"../lib/common_fun.h"
#include "httplib.h"
#include "order_client.h"
#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace httplib;
using namespace std;
extern string grpcServerIp;
extern int grpcServerPort;
string dump_headers(const Headers& headers);
string log(const Request& req, const Response& res);
int http_initial(Server& httpSvr, string serverIp="127.0.0.1", int port=50051);
void trim(string& s);
Json::Value split(const char* in);