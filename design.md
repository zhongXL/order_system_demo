# 设计文档
## 1.	简介
系统目标在为用户提供一个简易的订单系统，为商户提供“下单”和“查单”的功能。该系统基于C++ httplib库，对外使用restful协议开发API，对内使用grpc框架。
## 2.	框架设计
 ![image](https://github.com/zhongXL/order_system_demo/raw/master/picture/struct.jpg)
 <br> 
- 客户端发起http请求；
- http服务器接收http请求消息，将json数据转换成protobuf数据，之后将请求消息发送给业务服务器；
- 业务服务器接收请求消息，对数据库进行增查操作,之后发送http服务器响应消息。
- http服务器接收响应消息，将protobuf数据转化成json数据，之后将响应消息发送客户端。
## 3.	接口设计(参考微信支付商业平台API接口)
- path指该参数为路径参数
- query指该参数需在请求URL传参
- body指该参数需在请求JSON传参
### 3.1.	查单
请求URL：/v3/pay/transactions/out-trade-no/{out_trade_no}
<br> 请求方式：GET
<table>
	<tr>
		<td>参数名</td>
		<td>变量</td>
		<td>类型</td>
		<td>说明</td>
	<tr>
		<td>商户号</td>
		<td>mchid</td>
		<td>int</td>
		<td>query, 直连商户的商户号</td>
	<tr>
		<td>商户订单号</td>
		<td>out_trade_no</td>
		<td>string[6,32]</td>
		<td>path, 只能是数字、大小写字母_-*且在同一个商户号下唯一</td>
	</tr>
</table>

### 3.2.	下单
请求URL：/v3/pay/transactions/jsapi
<br>请求方式：POST
<table>
<tr>
		<td>参数名</td>
		<td>变量</td>
		<td>类型</td>
		<td>说明</td>
	</tr>
	<tr>
		<td>商户号</td>
		<td>mchid</td>
		<td>int</td>
		<td>body, 直连商户的商户号</td>
	</tr>
	<tr>
		<td>商户订单号</td>
		<td>out_trade_no</td>
		<td>string[6,32]</td>
		<td>body, 只能是数字、大小写字母_-*且在同一个商户号下唯一</td>
	</tr>
	<tr>
		<td>商品描述</td>
		<td>description</td>
		<td>string</td>
		<td>body, 商品描述</td>
	</tr>
	<tr>
		<td>通知地址</td>
		<td>notify_url</td>
		<td>String</td>
		<td>body, 符合URL规则</td>
	</tr>
	<tr>
		<td>订单支付金额</td>
		<td>amount</td>
		<td>object</td>
		<td>body, 订单金额信息包括total金额以及currency币种</td>
	</tr>
</table>

## 4.	对象模型设计
 <br> ![image](https://github.com/zhongXL/order_system_demo/raw/master/picture/class.jpg)
 <br> 
## 5.	数据库设计
### 5.1.	表设计
该系统将订单数据存储于MySQL数据库，订单表的设计如下：
 <br> ![image](https://github.com/zhongXL/order_system_demo/raw/master/picture/database.jpg)
 <br> 
### 5.2.	数据安全
- 避免从互联网访问MySQL,确保特定主机才拥有访问特权。该项目通过修改my.cnf强迫MySQL仅监听本机。
- 因为该系统只用于查单和下单功能，所以该项目降低用户对数据库的特权，仅给予用户查找和添加权限，而不应当给予修改删除权限。
- 为了防止应用程序用户输入使应用程序生成不期望的效果的语句的数值，该项目使用mysql_real_escape_string()API调用，对特殊字符做转义，以防SQL注入。
