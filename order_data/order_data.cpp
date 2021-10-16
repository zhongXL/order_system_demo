#include"order_data.h"
OrderData::OrderData(int merchant, int merchant_order , string description , string address , double amount , string currency )
{
	this->set_merchant_id(merchant);
	this->set_merchant_order_id(merchant_order);
	this->set_description(description);
	this->set_address(address);
	this->set_amount(amount);
	this->set_currency(currency);
}
void OrderData::set_merchant_id(int merchant)
{
	this->merchant_id = merchant;
}
int OrderData::get_merchant_id() const
{
	return this->merchant_id;
}
void OrderData::set_merchant_order_id(int merchant_order)
{
	this->merchant_order_id = merchant_order;
}
int OrderData::get_merchant_order_id() const
{
	return this->merchant_order_id;
}
void OrderData::set_description(string description)
{
	this->description = description;
}
string OrderData::get_description() const
{
	return this->description;
}
void OrderData::set_address(string address)
{
	this->address = address;
}
string OrderData::get_address() const
{
	return this->address;
}
void OrderData::set_amount(double amount)
{
	this->amount = amount;
}
double OrderData::get_amount() const
{
	return this->amount;
}
void OrderData::set_currency(string currency)
{
	this->currency = currency;
}
string OrderData::get_currency() const
{
	return this->currency;
}