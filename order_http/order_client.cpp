#include"order_client.h"

OrderClient::OrderClient(shared_ptr<Channel> channel)
    : stub_(Order::NewStub(channel)) {}

string OrderClient::AddOrder(const OrderData* data) {
    // Data we are sending to the server.
    OrderInformation information;
    information.set_merchant_id(data->get_merchant_id());
    information.set_merchant_order_id(data->get_merchant_order_id());
    information.set_description(data->get_description());
    information.set_address(data->get_address());
    information.set_amount(data->get_amount());
    information.set_currency(data->get_currency());


    // Container for the data we expect from the server.
    addOrderResult reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->AddOrder(&context, information, &reply);

    // Act upon its status.
    if (status.ok()) 
    {
        if (reply.result() == 0)
        {
            return "Add the order failed!";
        }
        else
        {
            return "Add the order success!";
        }
    }
    else 
    {
        cout << status.error_code() << ": " << status.error_message()
            << endl;
        return "RPC failed";
    }
}

string OrderClient::QueryOrder(const int& merchantID, const int& merchantOrderID, OrderInformation* reply) {
    // Data we are sending to the server.
    OrderID orderID;
    orderID.set_merchant_id(merchantID);
    orderID.set_merchant_order_id(merchantOrderID);

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->QueryOrder(&context, orderID, reply);

    // Act upon its status.
    if (status.ok()) {
        return "Query the order success!";
    }
    else {
        cout << status.error_code() << ": " << status.error_message()
            << endl;
        return "Query the order failed!";
    }
}