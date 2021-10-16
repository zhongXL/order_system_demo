#include "order_server.h"

bool OrderServiceImpl::connectDataBase(const char* userName, const char* password, const char* host, const char* name, unsigned int port)
{   
    database = new OrderDataBase(userName, password, host, name, port);
    return database->connectDataBase();
}

void OrderServiceImpl::freeConnect()
{
    database->freeConnect();
    delete database;
}

Status OrderServiceImpl::AddOrder(ServerContext* context, const OrderInformation* request,
    addOrderResult* reply){
    OrderData requestOrder;
    requestOrder.set_merchant_id(request->merchant_id());
    requestOrder.set_merchant_order_id(request->merchant_order_id());
    requestOrder.set_description(request->description());
    requestOrder.set_address(request->address());
    requestOrder.set_amount(request->amount());
    requestOrder.set_currency(request->currency());
    int result=database->addOrder(&requestOrder);
    reply->set_result(result);
    return Status::OK;
}

Status OrderServiceImpl::QueryOrder(ServerContext* context, const OrderID* request,
    OrderInformation* reply) {
    OrderData replyOrder;
    int result = database->findOrder(request->merchant_id(), request->merchant_order_id(), &replyOrder);
    reply->set_merchant_id(replyOrder.get_merchant_id());
    reply->set_merchant_order_id(replyOrder.get_merchant_order_id());
    reply->set_description(replyOrder.get_description());
    reply->set_address(replyOrder.get_address());
    reply->set_amount(replyOrder.get_amount());
    reply->set_currency(replyOrder.get_currency());
    return Status::OK;
}



