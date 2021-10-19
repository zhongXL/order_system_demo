#include<iostream>
#include"http_service.h"

int main(int argc, char* argv[])
{
   /*
   argv[1] : http server listen ip
   argv[2] : http server listen port
   argv[3] : grpc server ip
   argv[4] : grpc server port
   */

#ifdef cpphttplib_openssl_support
	sslserver svr(server_cert_file, server_private_key_file);
#else
	Server svr;
#endif

	if (!svr.is_valid()) {
		printf("server has an error...\n");
		return -1;
	}

	string httpServerListenIp = "0.0.0.0";
	int httpServerListenPort = 8080;
	string grpcServerIp = "127.0.0.1";
	int grpcServerPort = 50051;


	if (argc != 1 && argc != 5)
	{
		cout << "input format is wrong!";
		return 0;
	}
	if (argc == 5) 
	{
		//check input format is valid
       if (!isIP(argv[1])) 
       {
           cout << "http server listen ip is not in correct format!" << endl;
           return 0;
       }

       if (!isNumber(argv[2]) || (atoi(argv[2])) > 65535) 
       {
           cout << "http server listen port is not in correct format!" << endl;
           return 0;
       }

		if (!isIP(argv[3]))
		{
			cout << "grpc server ip is not in correct format!" << endl;
			return 0;
		}

		if (!isNumber(argv[4]) || (atoi(argv[4])) > 65535)
		{
			cout << "grpc server listen port is not in correct format!" << endl;
			return 0;
		}

		httpServerListenIp = argv[1];
		httpServerListenPort = atoi(argv[2]);
		grpcServerIp = argv[3];
		grpcServerPort = atoi(argv[4]);
	}

	cout << "http server is listening for requests at: " << httpServerListenIp << ":" << httpServerListenPort << endl;
	
	http_initial(svr, grpcServerIp, grpcServerPort);
	bool result = svr.listen(httpServerListenIp.c_str(), httpServerListenPort);
	
	return 0;
}

