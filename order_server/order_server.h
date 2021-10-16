#ifndef _ORDER_SERVER_H
#define _ORDER_SERVER_H
#endif
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "../order_proto/order.grpc.pb.h"
#include"../order_data/order_database.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using order_proto::OrderInformation;
using order_proto::addOrderResult;
using order_proto::OrderID;
using order_proto::Order;
using namespace std;

// Logic and data behind the server's behavior.
class OrderServiceImpl final : public Order::Service {
private:
    OrderDataBase* database;

public:
    bool connectDataBase(const char* userName = "root", const char* password = "123456", const char* host = "localhost", const char* name = "order_system", unsigned int port = 3306); //server connects dataBase;
    void freeConnect();
    Status AddOrder(ServerContext* context, const OrderInformation* request, addOrderResult* reply) override;
    Status QueryOrder(ServerContext* context, const OrderID* request, OrderInformation* reply) override;
};


