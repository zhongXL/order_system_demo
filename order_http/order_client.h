#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "httplib.h"
#include "../order_proto/order.grpc.pb.h"
#include "../order_data/order_data.h"
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using order_proto::OrderInformation;
using order_proto::addOrderResult;
using order_proto::OrderID;
using order_proto::Order;
using namespace std;

class OrderClient {
public:
    // Assembles the client's payload, sends it and presents the response back from the server.
    OrderClient(shared_ptr<Channel> channel);
   
    string AddOrder(const OrderData* data);

    string QueryOrder(const int& merchantID, const string& merchantOrderID, OrderInformation* reply);

private:
    unique_ptr<Order::Stub> stub_;
};

