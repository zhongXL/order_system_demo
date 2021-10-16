#ifndef _ORDER_DATA_H_
#define _ORDER_DATA_H_
#include<string>
using namespace std;
class OrderData 
{
private:
	int merchant_id;
	int merchant_order_id;
	string description;
	string address;
	double amount;
	string currency;
public:
	OrderData(int merchant = 0, int merchant_order = 0, string description = "", string address = "", double amount = 0, string currency = "RMB");
	void set_merchant_id(int merchant);
	int get_merchant_id() const;
	void set_merchant_order_id(int merchant_order);
	int get_merchant_order_id() const;
	void set_description(string description);
	string get_description() const;
	void set_address(string address);
	string get_address() const;
	void set_amount(double amount);
	double get_amount() const;
	void set_currency(string currency);
	string get_currency() const;
};
#endif