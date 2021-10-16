#include <string.h>
#include<iostream>
#include "order_server.h"
#include "../lib/common_fun.h"


void RunServer(OrderServiceImpl& service,string ip="0.0.0.0", string port="50051") {
    std::string server_address(ip+":"+port);
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}


//int main(int argc, char* argv[]) {
//    /*
//    argv[1] : server listen ip
//    argv[2] : server listen port
//    argv[3] : database user name
//    argv[4] : database password
//    argv[5] : database ip
//    argv[6] : database port
//    argv[7] : database name
//    */
//    if (argc != 1 && argc !=8)
//    {
//        cout << "input format is wrong!";
//        return 0;
//    }
//    OrderServiceImpl service;
//    if (argc == 1) 
//    {
//        if (!service.connectDataBase()) 
//        {
//            return 0;
//        }
//        RunServer(service);
//        service.freeConnect();
//    }
//    else 
//    {
//        //check input format is valid
//        if (!isIP(argv[1])) 
//        {
//            cout << " server listen ip is not in correct format!" << endl;
//            return 0;
//        }
//
//        if (!isNumber(argv[2]) || (atoi(argv[2])) > 65535) 
//        {
//            cout << " server listen port is not in correct format!" << endl;
//            return 0;
//        }
//
//        if (!isIP(argv[5]))
//        {
//            cout << " database  ip is not in correct format!" << endl;
//            return 0;
//        }
//
//        if (!isNumber(argv[6]) || (atoi(argv[6])) > 65535)
//        {
//            cout << " database port is not in correct format!" << endl;
//            return 0;
//        }
//        
//        if (!service.connectDataBase(argv[3], argv[4], argv[5], argv[7], atoi(argv[6]))) 
//        {
//            cout<<"database can not be connected!"<<endl;
//            return 0;
//        }
//        RunServer(service,argv[1],argv[2]);
//        service.freeConnect();
//    }
//    return 0;
//}