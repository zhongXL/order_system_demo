#include<iostream>
#include <iomanip>
#include<map>
#include<httplib.h>
#include<json/json.h>
#include"../lib/common_fun.h"

#define CA_CERT_FILE "./ca-bundle.crt"

void help() 
{
    cout << "common lists:" << endl;
    cout << setw(10) << left << "help" << "Display this help" << endl;
    cout << setw(10) << left << "add" << "add an order" << endl;
    cout << setw(10) << left << "query" << "query an order" << endl;
    cout << setw(10) << left << "quit" << "exit this program" << endl;
}


int main(int argc, char* argv[]) {
    /*
    argv[1] : http server  ip
    argv[2] : http server  port
    */
    string httpServerIp = "127.0.0.1";
    int httpServerPort = 8080;

    if (argc != 1 && argc != 3)
    {
        cout << "input format is wrong!";
        return 0;
    }
    if (argc == 3)
    {
        //check input format is valid
        if (!isIP(argv[1]))
        {
            cout << "http server ip is not in correct format!" << endl;
            return 0;
        }

        if (!isNumber(argv[2]) || (atoi(argv[2])) > 65535)
        {
            cout << "http server port is not in correct format!" << endl;
            return 0;
        }

        httpServerIp = argv[1];
        httpServerPort = atoi(argv[2]);

    }
    
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli(httpServerIp, httpServerPort);
    cli.set_ca_cert_path(CA_CERT_FILE);
    cli.enable_server_certificate_verification(true);
#else
    httplib::Client cli(httpServerIp, httpServerPort);
#endif

    help();
    cout << endl;
    string opera;
    while (1) 
    {
        cin >> opera;
        if (opera == "help") 
        {
            help();
            cout << endl;
        }
        else 
        {
            if (opera == "add")
            {
                Json::Value rootValue = Json::objectValue;
                string merchant_id;
                string merchant_order_id;
                string description;
                string address;
                string amount;
                string currency;
                string temp;
                string query;
                while (1)
                {
                    cout << "Please enter merchant ID (A non-negative integer): " << endl;
                    cin >> merchant_id;
                    if (isNumber(merchant_id)) 
                    {
                        break;
                    }
                }
                query = "merchant_id=" + merchant_id+"&";

                while (1) 
                {
                    cout << "Please enter merchant order ID (A non-negative integer): " << endl;
                    cin >> merchant_order_id;
                    if (isNumber(merchant_order_id)) 
                    {
                        break;
                    }
                }
                query = query + "merchant_order_id=" + merchant_order_id + "&";

                cout << "Please enter description about the order: " << endl;
                temp = "\n";
                getline(cin, temp);
                getline(cin, description);
                query = query + "description=" + description + "&";

                while (1) 
                {
                    cout << "Please enter an address for notice (An url): " << endl;
                    cin >> address;
                    if (isUrl(address)) 
                    {
                        break;
                    }
                }
                query = query + "address=" + address + "&";

                while (1) 
                {
                    cout << "Please enter an amount for the order(A non-negative number): " << endl;
                    cin >> amount;
                    if (isDouble(amount)) 
                    {
                        break;
                    }
                }
                query = query + "amount=" + amount + "&";

                cout << "Please enter the currency for the order: " << endl;
                cin >> currency;
                query = query + "currency=" + currency + "&";


                if (auto res = cli.Post("/order", query, "application/json")) {
                    cout << res->status << endl;
                    cout << res->get_header_value("Content-Type") << endl;
                    cout << res->body << endl;
                }
                else 
                {
                    cout << "error code: " << res.error() << std::endl;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
                        auto result = cli.get_openssl_verify_result();
                        if (result) {
                            cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
                        }
#endif
                }
                cout << endl;
            }

            else if (opera == "query")
            {
                string merchant_id;
                string merchant_order_id;
                while (1) 
                {
                    cout << "Please enter merchant ID (A non-negative integer) for the query order: " << endl;
                    cin >> merchant_id;
                    if (isNumber(merchant_id)) 
                    {
                        break;
                    }
                }
                
                while (1) 
                {
                    cout << "Please enter merchant order ID (Anon-negative integer) for the query order: " << endl;
                    cin >> merchant_order_id;
                    if (isNumber(merchant_order_id)) 
                    {
                        break;
                    }
                }
                string query = "/order?merchant_id=" + merchant_id + "&merchant_order_id=" + merchant_order_id;
                
                
                if (auto res = cli.Get(query.c_str())) {
                    cout << res->status << endl;
                    cout << res->get_header_value("Content-Type") << endl;
                    cout << res->body << endl;
                }
                else
                {
                    cout << "error code: " << res.error() << std::endl;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
                    auto result = cli.get_openssl_verify_result();
                    if (result) {
                        cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
                    }
#endif
                }
                cout << endl;
            }

            else if (opera == "quit") 
            {
                return 0;
            }
            help();
            cout << endl;
        }
    }
    return 0;
}