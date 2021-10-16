# 使用文档
## 1.	简介
<br>该项目实现一个简易的订单系统，为商户提供“查单”和“下单”的功能。</br>
<br>该工程在windows10 VS2019 C++下采用bazel编译。</br>
## 2.	项目安装
### 2.1	数据库
该项目需要使用MySQL数据库存储订单数据，因此需提前通过官网https://dev.mysql.com/downloads/workbench/8.0.html 下载安装mysql并创建数据库。可以直接通过导入order_data/order.sql文件创建数据表`order`。
### 2.2	编译
- 服务端编译指令：bazel build //order_server:main_server
- http服务端编译指令：bazel build //order_http:main_http
- 客户端编译指令：bazel build //order_client:main_client
## 3.	启动服务
### 3.1	启动数据库mysql
net -start mysql
<br>启动成功界面如下：

<br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/netStartMysql.png)

### 3.2	启动服务端
- 指定参数启动服务端：.\main_server.exe $ListenIP $ListenPort $DatabaseUserName $DatabasePassword $databaseIP $databasePort $databaseName
- 默认参数启动服务器：.\main_server.exe
<br>其中，
<table>
 <tr>
		<td>参数</td>
		<td>说明</td>
		<td>默认值</td>
	</tr>
 	<tr>
		<td>$ListenIP</td>
		<td>服务器监听IP地址</td>
		<td>0.0.0.0</td>
	</tr>
	<tr>
		<td>$ListenPort</td>
		<td>服务器监听端口号</td>
		<td>50051</td>
	</tr>
	<tr>
		<td>$DatabaseUserName</td>
		<td>数据库用户名</td>
		<td>root</td>
	</tr>
	<tr>
		<td>$DatabasePassword</td>
		<td>数据库用户密码</td>
		<td>123456</td>
	</tr>
	<tr>
		<td>$databaseIP</td>
		<td>数据库IP</td>
		<td>127.0.0.1</td>
	</tr>
	<tr>
		<td>$databasePort</td>
		<td>数据库端口号</td>
		<td>3306</td>
	</tr>
	<tr>
		<td>$databaseName</td>
		<td>数据库名</td>
		<td>order_system</td>
	</tr>
 </table>
 <br>
启动成功如下：

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/startMainServer.png)
 
### 3.3	启动http
- 指定参数启动http服务：.\main_http.exe $ListenIP $ListenPort $c $ServerPort
- 默认参数启动http服务：先启动http服务器：.\main_http.exe
<br>其中，
<table>
 <tr>
		<td>参数</td>
		<td>说明</td>
		<td>默认值</td>
	</tr>
	<tr>
		<td>$ListenIP</td>
		<td>http监听IP地址</td>
		<td>0.0.0.0</td>
	</tr>
	<tr>
		<td>$ListenPort</td>
		<td>http监听端口号</td>
		<td>8080</td>
	</tr>
	<tr>
		<td>$ServerIP</td>
		<td>服务器IP</td>
		<td>127.0.0.1</td>
	</tr>
	<tr>
		<td>$ServerPort</td>
		<td>服务器端口号</td>
		<td>50051</td>
	</tr>
 </table>
 <br>
启动成功如下：

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/startMainHttp.png)
 
### 3.4	启动客户端
- 指定参数启动客户端：.\main_client.exe $HttpIP $HttpPort
- 默认参数启动客户端: .\main_client.exe
<br>其中，
<table>
 <tr>
		<td>参数</td>
		<td>说明</td>
		<td>默认值</td>
	</tr>
	<tr>
		<td>$HttpIP</td>
		<td>提供http服务的IP地址</td>
		<td>127.0.0.1</td>
	</tr>
	<tr>
		<td>$HttpPort</td>
		<td>提供http服务的端口号</td>
		<td>8080</td>
	</tr>
 </table>
 <br>
启动成功如下：

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/startMainClient.png)
 
指令信息
<table>
 <tr>
		<td>指令</td>
		<td>功能</td>
		<td>说明</td>
	</tr>
	<tr>
		<td>help</td>
		<td>打印指令列表</td>
		<td>列出客户端支持的指令</td>
	</tr>
	<tr>
		<td>add</td>
		<td>添加订单</td>
		<td>根据输入信息添加相关订单</td>
	</tr>
	<tr>
		<td>query</td>
		<td>查询订单</td>
		<td>根据输入信息查询相关订单</td>
	</tr>
	<tr>
		<td>quit</td>
		<td>退出客户端</td>
		<td>退出客户端程序</td>
	</tr>
 </table>
 <br>
 

#### 3.4.1	help指令
用户输入“help”,将列出客户端支持的指令

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/help.png)

#### 3.4.2	add指令
用户输入”add”指令，将相继输入添加订单中的商户号、商户订单号、订单描述、通知地址、订单金额以及订单支付币种。其中，商户号和商户订单号仅支持非负整数，订单金额最大支持小数点后两位，通知地址需符合URL规则。

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/add.png)

#### 3.4.3	query指令
用户输入“query“指令，将相继输入查询订单的商户号以及商户订单号。其中，商户号和商户订单号仅支持非负整数。

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/query.png)
 
#### 3.4.4	quit指令
用户输入“quit”指令，将退出程序。

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/quit.png)

## 4.	注意事项
### 4.1	curl操作
- 查询订单支持curl操作：
curl http://127.0.0.1:8080/order?merchant_id=$1’&’merchant_order_id=$2
(其中,$1和$2为非负整数)

 <br> ![image](https://github.com/zhongXL/order_system_demo/blob/master/picture/curl.png)
 
- 添加订单暂不支持curl操作

