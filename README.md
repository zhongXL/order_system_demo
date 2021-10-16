使用文档
1.	简介
该项目实现一个简易的订单系统，为商户提供“查单”和“下单”的功能。该工程在windows10 VS2019 C++下采用bazel编译
2.	项目安装
2.1	数据库
该项目需要使用MySQL数据库存储订单数据，因此需提前通过官网https://dev.mysql.com/downloads/workbench/8.0.html下载安装mysql并创建数据库。可以直接通过导入order_data/order.sql文件创建数据表`order`。
2.2	编译
服务端编译指令：bazel build //order_server:main_server
http服务端编译指令：bazel build //order_http:main_http
客户端编译指令：bazel build //order_client:main_client
3.	启动服务
3.1	启动数据库mysql
net -start mysql
启动成功界面如下：
 
3.2	启动服务端
指定参数启动服务端：.\main_server.exe $ListenIP $ListenPort $DatabaseUserName $DatabasePassword $databaseIP $databasePort $databaseName
默认参数启动服务器：.\main_server.exe
其中，
参数	说明	默认值
$ListenIP	服务器监听IP地址	0.0.0.0
$ListenPort	服务器监听端口号	50051
$DatabaseUserName	数据库用户名	root
$DatabasePassword	数据库用户密码	123456
$databaseIP	数据库IP	127.0.0.1
$databasePort	数据库端口号	3306
$databaseName	数据库名	order_system
启动成功如下：
 
3.3	启动http
指定参数启动http服务：.\main_http.exe $ListenIP $ListenPort $ServerIP $ServerPort
默认参数启动http服务：先启动http服务器：.\main_http.exe
其中，
参数	说明	默认值
$ListenIP	http监听IP地址	0.0.0.0
$ListenPort	http监听端口号	8080
$DatabaseUserName	服务器IP	127.0.0.1
$DatabasePassword	服务器端口号	50051
启动成功如下：
 
3.4	启动客户端
指定参数启动客户端：.\main_client.exe $HttpIP $HttpPort
默认参数启动客户端: .\main_client.exe
其中，
参数	说明	默认值
$ HttpIP	提供http服务的IP地址	127.0.0.1
$ HttpPort	提供http服务的端口号	8080
启动成功如下：
 
指令信息
指令	功能	说明
help	打印指令列表	列出客户端支持的指令
add	添加订单	根据输入信息添加相关订单
query	查询订单	根据输入信息查询相关订单
quit	退出客户端	退出客户端程序
3.4.1	help指令
用户输入“help”,将列出客户端支持的指令
 

3.4.2	add指令
用户输入”add”指令，将相继输入添加订单中的商户号、商户订单号、订单描述、通知地址、订单金额以及订单支付币种。其中，商户号和商户订单号仅支持非负整数，订单金额最大支持小数点后两位，通知地址需符合URL规则。
 

3.4.3	query指令
用户输入“query“指令，将相继输入查询订单的商户号以及商户订单号。其中，商户号和商户订单号仅支持非负整数。
 
3.4.4	quit指令
用户输入“quit”指令，将退出程序。
 

4.	注意事项
4.1	curl操作
1).查询订单支持curl操作：
curl http://127.0.0.1:8080/order?merchant_id=$1’&’merchant_order_id=$2
(其中,$1和$2为非负整数)
 
2).添加订单暂不支持curl操作

