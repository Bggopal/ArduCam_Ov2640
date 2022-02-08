

N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

9 HTTP/HTTPS Commands

9.1 AT+HTTPPARA–Setting HTTP Parameters

To set HTTP parameters

Format

Type Command

Response

<CR><LF>OK<CR><LF>

Set

AT+HTTPPARA=<para>,<para\_value><CR> or

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 300ms.

Parameter

**<para>**

HTTP parameters, supporting the following two parameters:

url: Destination path

port: Destination port ID (no default value)

keepalive: set long-time connection

para\_value=0 default

para\_value=1 long-time connection

recvmode: receive mode

para\_value=0 default mode, only one +HTTPRECV: header is included in one HTTP

response

para\_value=1 data is displayed in format of +HTTPRECV: <length>,<data>

**<para\_value>** Value of **<para>**. The value of url contains at most 2048 bytes and url supports domain

name translation.

Copyright © Neoway Technology Co., Ltd

108





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Example

AT+HTTPPARA =url,www.neoway.com.cn/en/index.aspx

OK

Set the Neoway homepage as the URL. The

URL supports domain name translation.

AT+HTTPPARA=url,121.15.200.97/Service1.asmx/GetNote

OK

Set URL.

AT+HTTPPARA=url,

ERROR

The AT command is not complete.

Set the destination port ID to 80.

Set the destination port ID to 8080.

AT+HTTPPARA=port,80

OK

AT+HTTPPARA=port,8080

OK

9.2 AT+HTTPSETUP–Setting Up HTTP Connection

To set up an HTTP connection

The connection is set up successfully only after setting the destination address and port ID correctly.

Ensure that a network connection has been set up successfully before setting an HTTP connection.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

Execute AT+HTTPSETUP<CR>

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 30s.

Parameter

N/A.

Example

AT+HTTPSETUP

OK

Set up an HTTP connection

Successful

AT+HTTPSETUP

Set up an HTTP connection

Copyright © Neoway Technology Co., Ltd

109





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

ERROR

DNS translation fails

9.3 AT+HTTPACTION – Executing HTTP Request

To execute an HTTP request

Comply with the HTTP protocol when defining packets.

Add a carriage return to the end of the packets if the HTTP request is set to custom packet mode.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

<CR><LF>ERROR<CR><LF>

Or

<CR><LF>><post\_content><CR>

<LF>

⚫

⚫

AT+HTTPACTION=<mode>[,<length>[,<ty

pe>]]<CR>

Execute

<CR><LF>OK<CR><LF>

Or

AT+HTTPACTION=<mode>[,<offset>,<siz

e>]<CR>

<CR><LF>+HTTPACTION:SOCK

ET ID OPEN FAILED<CR><LF>

Or

<CR><LF>+HTTPSEND:

ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 30s.

Parameter

**<mode>**

HTTP request mode, available value can be 0, 1, 2, 99

0: GET

1: HEAD

2: POST

99: OPEN\_MODE, custom packet mode

POST content length or custom packet length; mandatory when <mode> is set

to POST or OPEN\_MODE, 2048 at most.

data type of POST request

**<length>**

**<type>**

Copyright © Neoway Technology Co., Ltd

110





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

0: x-www-form-url encoded

1: text

2: json

3: xml

4: html

**<offset>**

**<size>**

Offset in GET mode

Size of file to be downloaded in GET mode

**<post\_content>** Content sent through HTTPPOST

Example

AT+HTTPPARA=url,www.neoway.com.cn/en/index.aspx

Set the destination

path.

OK

Set up an HTTP

connection.

AT+HTTPSETUP

OK

AT+HTTPACTION=0

OK

GET request

+HTTPRECV:

Receive the response

from the HTTP server.

HTTP/1.1 200 OK

Cache-Control: private

Content-Type: text/html; charset=utf-8

Server: Microsoft-IIS/7.5

Set-Cookie: ASP.NET\_SessionId=rh3fjg554ufzb145aevgzz45; path=/;

HttpOnly

X-AspNet-Version: 2.0.50727

X-Powered-By: ASP.NET

X-UA-Compatible: IE=EmulateIE7

Date: Wed, 02 Mar 2016 06:52:35 GMT

Connection: close

Content-Length: 13842

The server finishes the

response and

/\*neoway homepage, html format, 13842 bytes\*/

……..

disconnects the

connection.

/\* neoway homepage\*/

+HTTPCLOSED: HTTP Link Closed

AT+HTTPPARA =url,www.neoway.com.cn/en/index.aspx

Set the destination

path.

OK

Set up an HTTP

connection

AT+HTTPSETUP

OK

HEAD request

AT+HTTPACTION=1

OK

The HTTP server

responds.

+HTTPRECV:

HTTP/1.1 200 OK

Cache-Control: private

Content-Length: 13842

Content-Type: text/html; charset=utf-8

Copyright © Neoway Technology Co., Ltd

111





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Server: Microsoft-IIS/7.5

Set-Cookie: ASP.NET\_SessionId=znt4fqabqsuclz55pvfufn55; path=/;

HttpOnly

X-AspNet-Version: 2.0.50727

X-Powered-By: ASP.NET

X-UA-Compatible: IE=EmulateIE7

Date: Thu, 28 Nov 2013 03:32:35 GMT

Connection: close

+HTTPCLOSED: HTTP Link Closed

AT+HTTPPARA=url,121.15.200.97/Service1.asmx/GetNote

Set destination path

OK

AT+HTTPPARA=port,8080

Set the destination

port ID as 8080.

OK

Set up an HTTP

connection

AT+HTTPSETUP

OK

AT+HTTPACTION=2,23

\>MAC=NEOWAY&DATA=0123456

OK

POST request.

Send 23 bytes; enter

the contents to be

uploaded after > is

displayed.

+HTTPRECV:

HTTP/1.1 200 OK

Cache-Control: private, max-age=0

Content-Type: text/xml; charset=utf-8

Server: Microsoft-IIS/7.5

X-AspNet-Version: 4.0.30319

X-Powered-By: ASP.NET

Receive the response

from the HTTP server.

The server replies an

XML file containing the

uploaded content NEOWAY

and 0123456.

Date: Thu, 28 Nov 2013 03:41:52 GMT

Connection: close

Content-Length: 98

<?xml version="1.0" encoding="utf-8"?>

<string xmlns="http://wsliu.cn/">NEOWAY+0123456

</string>

The server disconnected

with the module after

it finished responding.

+HTTPCLOSED: HTTP Link Closed

AT+HTTPPARA=url,www.neoway.com.cn/en/index.aspx

Set destination path

OK

AT+HTTPSETUP

The HTTP connection is

set up through port 80.

OK

AT+HTTPACTION=99,76

\>HEAD /en/index.aspx HTTP/1.1

connection: close

HOST: www.neoway.com.cn

Send 76-byte user-

defined packets

OK

+HTTPRECV:

Copyright © Neoway Technology Co., Ltd

112





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

HTTP/1.1 200 OK

Cache-Control: private

Content-Length: 13842

Receive the response

from the HTTP server.

Content-Type: text/html; charset=utf-8

Server: Microsoft-IIS/7.5

Set-Cookie: ASP.NET\_SessionId=pvlaai3fizxg44eyvyqsyenk; path=/;

HttpOnly

X-AspNet-Version: 2.0.50727

X-Powered-By: ASP.NET

X-UA-Compatible: IE=EmulateIE7

Date: Thu, 28 Nov 2013 05:40:24 GMT

Connection: close

The server disconnects

with the module after

it finishes responding.

+HTTPCLOSED: HTTP Link Closed

PPP is not enabled or

SOC connection

AT+HTTPACTION=0

+HTTPACTION: SOCKET ID OPEN FAILED

encountered an error.

AT+HTTPACTION=0

Failed to send data.

Other errors

+HTTPSETUP: ERROR

AT+HTTPACTION=2,adasd

ERROR

9.4 AT+HTTPCLOSE–Closing an HTTP Socket

To close an HTTP socket

After the **+HTTPCLOSE** command is sent, the HTTP socket is closed and the setting of +HTTPPARA

is cleared.

Only **OK** is returned after running this command if the HTTP socket is not connecting.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

Execute

AT+HTTPCLOSE<CR>

<CR><LF>ERROR<CR><LF>

Unsolicited

report

+HTTPCLOSE: <result>

Copyright © Neoway Technology Co., Ltd

113





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Timeout

The command times out if the module does not respond in 300ms.

Parameter

<result>

HTTP Link Closed: HTTP link is closed

ERROR: Fails to close the link

Example

AT+HTTPCLOSE

OK

Close the HTTP socket.

Unsolicited report after socket is closed

successfully.

+HTTPCLOSE: HTTP Link Closed

AT+HTTPCLOSE

OK

The HTTP socket is not connecting; only OK is

returned

9.5 +HTTPRECV–Unsolicited HTTP Data Output

Unsolicited HTTP data output

When the module receives HTTP data from the network, the UART prints the data automatically.

Format

Type

Command

⚫

⚫

+HTTPRECV: <datas>

+HTTPRECV: <length>,<datas>

Unsolicited report

Timeout

N/A.

Parameter

**<length>**

**<data>**

<length>

Data received through the HTTP socket

Copyright © Neoway Technology Co., Ltd

114





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Example

+HTTPRECV:

Report the data received

from the HTTP connection.

HTTP/1.1 200 OK

Cache-Control: private

Content-Length: 13842

Content-Type: text/html; charset=utf-8

Server: Microsoft-IIS/7.5

Set-Cookie: ASP.NET\_SessionId=pvlaai3fizxg44eyvyqsyenk; path=/;

HttpOnly

X-AspNet-Version: 2.0.50727

X-Powered-By: ASP.NET

X-UA-Compatible: IE=EmulateIE7

Date: Thu, 28 Nov 2013 05:40:24 GMT

Connection: close

+HTTPCLOSED: HTTP Link Closed

+HTTPRECV: 803,HTTP/1.1 206 Partial Content

Cache-Control: no-cache

Data format when RECVMODE

is set to 1

Connection: Keep-Alive

Content-Length: 10

Content-Range: bytes 0-9/14615

Content-Type: text/html

Date: Tue, 10 Jul 2018 00:55:30 GMT

Etag: "5b3c3650-3917"

Last-Modified: Wed, 04 Jul 2018 02:52:00 GMT

P3p: CP=" OTI DSP COR IVA OUR IND COM "

Pragma: no-cache

Server: BWS/1.1

Set-Cookie: BAIDUID=F18E6894A34321D8CF9AAF28C14FACC9:FG=1;

expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/;

domain=.baidu.com

Set-Cookie: BIDUPSID=F18E6894A34321D8CF9AAF28C14FACC9;

expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/;

domain=.baidu.com

Set-Cookie: PSTM=1531184130; expires=Thu, 31-Dec-37 23:55:55 GMT;

max-age=2147483647; path=/; domain=.baidu.com

Vary: Accept-Encoding

X-Ua-Compatible: IE=Edge,chrome=1

<!DOCTYPE

9.6 AT+HTTPGET–Downloading HTTP File

To download HTTP file.

This command is asynchronous. OK is returned immediately after the command is executed. The

downloading, decompressing, and verification processes are done in background.

Copyright © Neoway Technology Co., Ltd

115





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

AT+HTTPGET=<type>[,<check\_ty

pe>,<check\_value>]<CR>

Execute

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<type>**

Decompression type

0: unzip (default)

1: zip (unsupported)

2 to 99: reserved.

**<check\_type>**

Type of checking the integrity of compressed passage.

0: MD5 (default)

1 to 99: reserved.

**<check\_value>** Check code, used with the check type.

Example

AT+HTTPPARA=url,

Download the file.

mybank.icbc.com.cn/icbc/perbank/index.jsp

OK

AT+HTTPGET=0

OK

+HTTPGETRPT: 10

AT+HTTPGET=1

OK

+HTTPGETRPT: 20

AT+HTTPGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+HTTPGETRPT: 30,303

AT+HTTPGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

Copyright © Neoway Technology Co., Ltd

116





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

⚫

⚫

When **<check\_type>** and **<check\_value>** are ignored, no verification will be performed after the download

is complete.

The downloading, decompressing, and verification results are reported through **+HTTPGETSTAT**. For

details, see its definitions.

9.7 +HTTPGETRPT–Unsolicited Download Result Report

Unsolicited Download Result Report.

This command indicates the execution result of **AT+HTTPGET**.

Different result codes are reported according different results during downloading, verification, and

decompression processes.

Format

Type

Command

Unsolicited report

\+ HTTPGETRPT: <state\_type>[,<err\_code>]

Timeout

N/A.

Parameter

**<state\_type>** State type

10: successful download

11: failed download

20: successful decompression

21: failed decompression

30: successful verification

31: failed verification

Error response encountered during HTTP GET.

301 Moved Permanently

302 Found

**<err\_code>**

303 See Other

304 Not Modified

305 Use Proxy

307 Temporary Redirect

400 Bad Request

Copyright © Neoway Technology Co., Ltd

117





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

401 Unauthorized

402 Payment Required

403 Forbidden

404 Not Found

405 Method Not Allowed

406 Not Acceptable

407 Proxy Authentication Required

408 Request Timeout

409 Conflict

410 Gone

411 Length Required

412 Precondition Failed

413 Payload Too Large

414 URI Too Long

415 Unsupported Media Type

416 Requested Range Not Satisfiable

417 Expectation Failed

500 Internal Server Error

501 Not Implemented

502 Bad Gateway

503 Service Unavailable

504 Gateway Timeout

505 HTTP Version Not Supported

Example

AT+HTTPPARA=url,

Download the file.

mybank.icbc.com.cn/icbc/perbank/index.jsp

AT+HTTPGET=0

OK

+HTTPGETRPT: 10

AT+HTTPGET=1

OK

+HTTPGETRPT: 20

AT+HTTPGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+HTTPGETRPT: 30,303

AT+APHTTPGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+APHTTPGETRPT: 31

Copyright © Neoway Technology Co., Ltd

118





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

9.8 AT+HTTPGETSTAT?–Querying Downloading Result

To query the HTTP GET process and the download result.

Format

Type

Command

Response

<CR><LF>

+HTTPGETSTAT:

Query AT+HTTPGETSTAT?<CR>

<state\_type>[,<err\_code>]<CR><LF>

<CR><LF>OK<CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<state\_type>** State type

0: unknown result

10: successful download

11: failed download

20: successful decompression

21: failed decompression

30: successful verification

31: failed verification

**<err\_code>**

Error response encountered during HTTP GET.

301 Moved Permanently

302 Found

303 See Other

304 Not Modified

305 Use Proxy

307 Temporary Redirect

400 Bad Request

401 Unauthorized

402 Payment Required

403 Forbidden

404 Not Found

405 Method Not Allowed

406 Not Acceptable

407 Proxy Authentication Required

408 Request Timeout

409 Conflict

Copyright © Neoway Technology Co., Ltd

119





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

410 Gone

411 Length Required

412 Precondition Failed

413 Payload Too Large

414 URI Too Long

415 Unsupported Media Type

416 Requested Range Not Satisfiable

417 Expectation Failed

500 Internal Server Error

501 Not Implemented

502 Bad Gateway

503 Service Unavailable

504 Gateway Timeout

505 HTTP Version Not Supported

Example

AT+HTTPGET=0

OK

AT+HTTPGETSTAT?

+HTTPGETSTAT: 10,303

OK

AT+APHTTPGET=1

+HTTPGETSTAT: 30,303

OK

9.9 +HTTPCLOSED–HTTP Socket Closed

Unsolicited report of the HTTP socket closing

Format

Type

Command

Unsolicited report

<CR><LF>+HTTPCLOSE: Link Closed<CR><LF>

Timeout

N/A

Copyright © Neoway Technology Co., Ltd

120





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Parameter

N/A

Example

+HTTPCLOSED: HTTP Link Closed

Unsolicited report of the HTTP socket closing

9.10 AT+HTTPSCFG–Configuring SSL Parameters for

HTTPS

To configure SSL parameters for HTTPS.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

AT+HTTPSCFG=<type>,<type\_n

ame><CR>

Execute

<CR><LF>ERROR<CR><LF>

<CR><LF>+HTTPSCFG: <type>,<type\_name>

<CR><LF>OK<CR><LF>

Query

Test

AT+HTTPSCFGA?<CR>

AT+HTTPSCFGA=?<CR>

<CR><LF>+HTTPSCFG:

<sslversion>,<authmode>,<cacert>,<clientcert>,<

clientkey>

<CR><LF>OK<CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<type>**

SSL options

**sslversion**: SSL version

**authmode**: authentication mode

**ciphersuite**: Cipher suite

**cacert**: CA certificate

**clientcert**: Client certificate

Copyright © Neoway Technology Co., Ltd

121





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

**clientkey**: Client key

**<type\_name>** setting of SSL

sslversion:

0: SSL3.0

1: TLS1.0

2: TLS1.1

3: TLS1.2

Authmode:

**0**: No authentication

**1**: Manage server authentication

**2**: Manage server and client authentication if requested by the remote server

**Cacert**: string type, CA certificate

**Clientcert**: string type, client certificate

**Clientkey**: string type, client key

If **authmode** is set to **0**, you do not have to set **cacert**, **clientcert**, or **clientkey**..

Example

AT+ HTTPSCFG =”sslversion”,0

Set SSL version to ssl3.0.

Set no authentication.

OK

AT+ HTTPSCFG =”authmode”,0

OK

AT+ HTTPSCFG?

+HTTPSCFG: 0,1,,ca.pem,cc.pem,ck.pem

Query current SSL settings.

OK

AT+ HTTPSCFG =?

\+ HTTPSCFG: <type>,<type\_name>

OK

Query the value range of parameters.

9.11 AT+HTTPSPARA–Setting HTTPS Parameters

To set HTTPS parameters.

Set new HTTPS parameters for new HTTPS requests.

After the **+HTTPSCLOSE** command is sent, the connection is closed and parameter settings will be

cleared.

Copyright © Neoway Technology Co., Ltd

122





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Execute AT+HTTPSPARA=<para>,<para\_value><CR> Or

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

<para>

HTTPS parameters, supporting the following two parameters:

url: Destination path

port: Destination port ID

keepalive: set long-time connection

recvmode: receive mode

para\_value=0 default mode, only one +HTTPRECV: header is included in one

HTTP response

para\_value=1 data is displayed in format of +HTTPRECV: <length>,<data>

The value of **<para>**. The value of **url** contains at most 512 bytes and **url** supports

domain name translation.

<para\_value>

Example

AT+HTTPSPARA=url,www.alipay.com/index.html

OK

Set the alipay homepage as the URL. The URL

supports domain name translation.

AT+HTTPSPARA=url,support.cdmatech.com/index.html

Set URL.

OK

AT+HTTPSPARA=port,443

OK

Set the destination port ID to 443.

9.12 AT+HTTPSSETUP–Setting up HTTPS Connection

To set up an HTTPS connection.

The connection is set up successfully only after setting the destination address and port ID correctly.

Ensure that PPP dialing is successful before an HTTPS connection is set up.

Copyright © Neoway Technology Co., Ltd

123





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

Execute

AT+HTTPSSETUP<CR>

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 30s.

Parameter

N/A.

Example

AT+HTTPSSETUP

OK

Set up an HTTPS connection

Successful

AT+HTTPSSETUP

ERROR

Set up an HTTPS connection

Failed.

9.13 AT+HTTPSACTION–Executing HTTPS Request

To execute an HTTPS request.

Comply with the HTTP protocol when defining packets.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

⚫

⚫

AT+HTTPSACTION=<mode>[,<length>[,<type>]<CR>

AT+HTTPSACTION=<mode>[,<offset>,<size>]<CR>

Execute

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 30s.

Copyright © Neoway Technology Co., Ltd

124





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Parameter

<mode>

HTTPS request mode, available value can be 0, 1, 2, 99

0: GET

1: HEAD

2: POST

99: OPEN\_MODE, custom packet mode

<length>

<type>

POST content length, ranging from 1 to 2048; or custom packet length when

**<mode>** is set to **POST** or **OPEN\_MODE**

data type of POST request

0: x-www-form-urlencoded

1: text

2: json

3: xml

4: html

<offset>

<size>

specifies the start of file to be downloaded in GET mode

specifies download size for file download in GET mode

Example

AT+HTTPSPARA=url,support.cdmatech.com/login/

Set the destination path.

OK

AT+HTTPSPARA=port,443

Set the destination port.

Set up an HTTPS connection.

OK

AT+HTTPSSETUP

OK

AT+HTTPSACTION=0

OK

+HTTPSRECV:

GET request.

HTTP/1.1 200 OK

Server: QUALCOMM

Receive the request from the HTTPS

server.

X-Powered-By: Servlet/2.5 JSP/2.1

Content-Type: text/html; charset=ISO-8859-1

Date: Sat, 15 Feb 2014 05:58:54 GMT

Content-Length: 7630

Connection: close

Set-Cookie:

The server responded and then

disconnected the connection.

JSESSIONID=8V1dS1CpzlPcyNl2LzJZLQgDxWclpMJzP3FHZhVhpGb8

3GVM02sn!1955538012; path=/; HttpOnly

/\*homepage, html format\*/

……..

/\*homepage\*/

+HTTPSCLOSED: HTTPS Link Closed

AT+HTTPSPARA=url,support.cdmatech.com/login/

Set the destination path.

OK

AT+HTTPSPARA=port,443

Set the destination port.

Set up an HTTPS connection.

OK

AT+HTTPSSETUP

OK

AT+HTTPSACTION=1

OK

+HTTPSRECV:

Copyright © Neoway Technology Co., Ltd

125





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

HTTP/1.1 200 OK

HEAD request

Server: QUALCOMM

X-Powered-By: Servlet/2.5 JSP/2.1

Content-Type: text/html; charset=ISO-8859-1

Date: Sat, 15 Feb 2014 06:05:39 GMT

Content-Length: 0

The HTTPS server responds.

Connection: close

Set-Cookie:

JSESSIONID=qyNVS1DSmnjS9cvh72yW1xz1jtjBBRj0yv0zTmMy2LVy

BG7HK02b!1955538012; path=/; HttpOnly

+HTTPSCLOSED: HTTPS Link Closed

AT+HTTPSPARA=url,mybank.icbc.com.cn/icbc/perbank/index.

Set the destination path.

jsp

OK

Set the port.

AT+HTTPSPARA=port,443

Set up an HTTPS connection.

OK

AT+HTTPSSETUP

OK

AT+HTTPSACTION=99,500

\>POST /icbc/perbank/index.jsp HTTP/1.1<CRLF> /\*custom

header\*/

Send 69-byte custom packets.

The HTTPS server responded.

Connection: close<CRLF>

Host: mybank.icbc.com.cn<CRLF>

Content-Length: 10<CRLF> /\* custom header \*/

/\* custom header \*/

/\* custom header \*/

Content-Type: application/x-www-form-urlencoded<CRLF>

/\* custom header \*/

<CRLF><CRLF>

/\*data to be sent\*/

……

The server closed the link after

responding.

+HTTPSRECV:

/\*homepage, html format\*/

……..

/\*homepage\*/

+HTTPSCLOSED: HTTPS Link Closed

9.14 AT+HTTPSCLOSE–Closing HTTPS Socket

To close an HTTPS socket.

After the **+HTTPSCLOSE** command is sent, the HTTPS socket is closed and the setting of

**+HTTPPARA** is cleared.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

Execute

AT+HTTPSCLOSE<CR>

<CR><LF>ERROR<CR><LF>

Copyright © Neoway Technology Co., Ltd

126





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

N/A.

Example

AT+HTTPSCLOSE

OK

Close the HTTPS socket.

+HTTPSCLOSE: HTTPS Link Closed

9.15 AT+HTTPSRECV–Unsolicited HTTPS Data Output

Unsolicited HTTPS data output

When the module receives HTTPS data from the client, the UART prints the data automatically.

Format

Type

Command

⚫

⚫

<CR><LF>+HTTPSRECV: <CR><LF><datas>

<CR><LF>+HTTPSRECV: <length>,<datas>

Unsolicited report

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

<datas>

Data received through the HTTPS connection.

Example

+HTTPSRECV:

Report the data received

from the HTTPS connection.

HTTP/1.1 200 OK

Copyright © Neoway Technology Co., Ltd

127





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Cache-Control: private

Content-Length: 13842

Content-Type: text/html; charset=utf-8

Server: Microsoft-IIS/7.5

Set-Cookie: ASP.NET\_SessionId=pvlaai3fizxg44eyvyqsyenk; path=/;

HttpOnly

X-AspNet-Version: 2.0.50727

X-Powered-By: ASP.NET

X-UA-Compatible: IE=EmulateIE7

Date: Thu, 28 Nov 2013 05:40:24 GMT

Connection: close

+HTTPSCLOSED: HTTPS Link Closed

+HTTPSRECV: 832,HTTP/1.1 206 Partial Content

Server: Tengine/2.1.0

Data format when RECVMODE

is set to 1

Date: Tue, 10 Jul 2018 01:09:25 GMT

Content-Type: text/html; charset=utf-8

Content-Length: 10

Connection: keep-alive

x-server-id: 40-5005

request-id: 0bea4b2215311849654971530e6674

Accept-Ranges: bytes

set-cookie: ctoken=MBHI38pHhdL6q0ltGFqjkviz; path=/;

domain=.alipay.com; secure

set-cookie:

ALIPAYJSESSIONID=jMi6e4Q2JmIN8HRk68wm53KXisfnB5H0homeproxy; path=/;

domain=.alipay.com

x-frame-options: SAMEORIGIN

x-xss-protection: 1; mode=block

x-content-type-options: nosniff

x-download-options: noopen

strict-transport-security: max-age=31536000

Content-Range: bytes 0-9/21651

x-readtime: 2

Set-Cookie: ssl\_upgrade=0;path=/;secure;

Set-Cookie:

spanner=aGuTtGMbvBcOy1dCyZ/e4JI97JSiPcR1Xt2T4qEYgj0=;path=/;secure;

Via: spanner-internet-g2-35.em14[206]

9.16 AT+HTTPSGET–Donwnloading HTTPS File

To download an HTTPS file.

This command is asynchronous. OK is returned immediately after the command is executed. The

downloading, decompressing, and verification processes are done in background.

When **<check\_type>** and **<check\_value>** are ignored, no verification will be performed after the

download is completed.

The downloading, decompressing, and verification results are reported through +HTTPSGETSTAT. For

details, see its definitions.

Copyright © Neoway Technology Co., Ltd

128





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

AT+HTTPSGET=<type>[,<check\_type>,<c

heck\_value>]<CR>

Execute

<CR><LF>ERROR<CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<type>**

Decompression type

0: unzip (default)

1: zip (unsupported)

2 to 99: reserved.

**<check\_type>**

Type of checking the integrity of compressed passage.

0: MD5 (default)

1 to 99: reserved.

**<check\_value>** Check code, used with the check type.

Example

AT+HTTPSPARA=url,

Download the file.

mybank.icbc.com.cn/icbc/perbank/index.jsp

OK

AT+HTTPSGET=0

OK

+HTTPSGETRPT: 10

AT+HTTPSGET=1

OK

+HTTPSGETRPT: 20

AT+HTTPSGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+HTTPSGETRPT: 30,303

AT+HTTPSGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+APHTTPSGETRPT: 31

Copyright © Neoway Technology Co., Ltd

129





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

9.17 +HTTPSGETRPT–Unsolicited Download Result

Report

Unsolicited Download Result Report.

Format

Type

Command

+HTTPSGETRPT: <state\_type>[,<err\_code>]

Unsolicited report

Timeout

N/A.

Parameter

**<state\_type>** State type

10: successful download

11: failed download

20: successful decompression

21: failed decompression

30: successful verification

31: failed verification

Error response encountered during HTTP GET.

301 Moved Permanently

302 Found

**<err\_code>**

303 See Other

304 Not Modified

305 Use Proxy

307 Temporary Redirect

400 Bad Request

401 Unauthorized

402 Payment Required

403 Forbidden

404 Not Found

405 Method Not Allowed

406 Not Acceptable

407 Proxy Authentication Required

408 Request Timeout

409 Conflict

410 Gone

Copyright © Neoway Technology Co., Ltd

130





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

411 Length Required

412 Precondition Failed

413 Payload Too Large

414 URI Too Long

415 Unsupported Media Type

416 Requested Range Not Satisfiable

417 Expectation Failed

500 Internal Server Error

501 Not Implemented

502 Bad Gateway

503 Service Unavailable

504 Gateway Timeout

505 HTTP Version Not Supported

Example

AT+HTTPSPARA=url,

Download the file.

mybank.icbc.com.cn/icbc/perbank/index.jsp

OK

AT+HTTPSGET=0

OK

+APHTTPSGETRPT: 10

AT+HTTPSGET=1

OK

+HTTPSGETRPT: 20

AT+HTTPSGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+HTTPSGETRPT: 30,303

AT+HTTPSGET=1,0,eaf84487e190bc79af55c972bbc63e3f

OK

+HTTPSGETRPT: 31

9.18 AT+HTTPSGETSTAT?–Querying Download Result

To query the HTTPS GET process and the download result.

Copyright © Neoway Technology Co., Ltd

131





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Format

Type

Command

Response

<CR><LF>

<state\_type>[,<err\_code>]<CR><LF>

+HTTPSGETSTAT:

Execute

AT+HTTPSGETSTAT?<CR>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<state\_type>** State type

0: unknown result

10: successful download

11: failed download

20: successful decompression

21: failed decompression

30: successful verification

31: failed verification

**<err\_code>**

Error response encountered during HTTP GET.

301 Moved Permanently

302 Found

303 See Other

304 Not Modified

305 Use Proxy

307 Temporary Redirect

400 Bad Request

401 Unauthorized

402 Payment Required

403 Forbidden

404 Not Found

405 Method Not Allowed

406 Not Acceptable

407 Proxy Authentication Required

408 Request Timeout

409 Conflict

410 Gone

411 Length Required

412 Precondition Failed

413 Payload Too Large

414 URI Too Long

415 Unsupported Media Type

416 Requested Range Not Satisfiable

Copyright © Neoway Technology Co., Ltd

132





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

417 Expectation Failed

500 Internal Server Error

501 Not Implemented

502 Bad Gateway

503 Service Unavailable

504 Gateway Timeout

505 HTTP Version Not Supported

Example

AT+HTTPSGET=0

OK

AT+HTTPSGETSTAT?

+HTTPSGETSTAT: 10,303

OK

AT+APHTTPSGET=1

OK

+HTTPSGETSTAT: 30,303

OK

9.19 AT+FILEHTTPACTION–HTTP Request in File

System

HTTP Request in File System.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

⚫

⚫

AT+FILEHTTPACTION=<mode>,<l

<CR><LF>ERROR<CR><LF>

Or

ength>,<type>,<dir&filename><CR

\>

Execute

<CR><LF>+CME

ERROR:<errcode><CR><LF>

Or

AT+FILEHTTPACTION=<mode>,<

offset>,<size><CR>

<CR><LF>+CME

ERROR:<errtext><CR><LF>

Copyright © Neoway Technology Co., Ltd

133





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<mode>**

HTTP request method.

0: GET

1: POST

**<length>**

**<type>**

Length of the POST content, supports 524288 bytes at most.

POST request data type.

0: x-www-form-urlencoded

1: text

2: json

3: xml

4: html

**<offset>**

**<size>**

Offset, specifies where file data starts.

Length of file downloaded from the start point, 524288 bytes at most.

**<dir&filename>** Path and name of the file required. The file path is relative to the root path of the

file system.

**<errcode>**

**<errtext>**

See Appendix A.

See Appendix A.

⚫

Establish a HTTP connection before executing this command.

⚫

Before downloading files through HTTP GET, ensure that there are enough remaining space in the file

system.

Example

AT+FILEHTTPACTION=0,0,524288

OK

Download 512 KB data, starting from the first

byte.

+FILEHTTPSTAT: 0,1,524288

AT+FILEHTTPACTION=1,524288,0,text.txt

OK

Adopt x-www-form-urlencoded type to post text.txt

file.

The file with 524288 data length is posted

successfully.

+FILEHTTPSTAT: 1,1,524288

AT+FILEHTTPACTION=0,0,524288

+CME ERROR: 1001

Failed command execution. 1001 indicates PDP not

active.

Copyright © Neoway Technology Co., Ltd

134





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

9.20 AT+FILEHTTPSACTION–HTTPS Request in File

System

HTTPS Request in File System.

Format

Type

Command

Response

<CR><LF>OK<CR><LF>

Or

<CR><LF>ERROR<CR><LF>

Or

AT+FILEHTTPSACTION=<mode>,<length>,<type>,

<dir&filename><CR>

Execute

<CR><LF>+CME

ERROR:<errcode><CR><LF>

Or

AT+FILEHTTPSACTION=<mode>,<offset>,<size><

CR>

<CR><LF>+CME

ERROR:<errtext><CR><LF>

Timeout

The command times out if the module does not respond in 300 ms.

Parameter

**<mode>**

HTTP request method.

0: GET

1:POST

**<length>**

**<type>**

Length of the POST content, supports 524288 bytes at most.

POST request data type.

0: x-www-form-urlencoded

1: text

2: json

3: xml

4: html

**<offset>**

**<size>**

Offset, specifies where file data starts.

Length of file downloaded from the start point, 524288 bytes at most.

**<dir&filename>** Path and name of the file required. The file path is relative to the root path of the

file system.

**<errcode>**

**<errtext>**

See Appendix A.

See Appendix A.

Copyright © Neoway Technology Co., Ltd

135





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

Example

AT+FILEHTTPSACTION=0,0,524288

OK

Download 512 KB data, starting from the first byte.

The file with 512KB data length is posted successfully.

+FILEHTTPSTAT: 0,1,524288

AT+FILEHTTPSACTION=1,524288,0,text.txt

OK

Adopt x-www-form-urlencoded type to post text.txt file.

The file with 524288 data length is posted

successfully.

+FILEHTTPSTAT: 1,1,524288

AT+FILEHTTPSACTION=0,0,524288

+CME ERROR: 1001

Failed command execution. 1001 indicates PDP not

active.

⚫

⚫

Establish a HTTP connection before executing this command.

Before downloading files through HTTP GET, ensure that there are enough remaining space in the file

system.

9.21 AT+FILEHTTPSTAT–Unsolicited HTTP(S) Result

Status Report in File System

Unsolicited HTTP(S) upload/download result status report in the file system.

Format

Type

Command

⚫

⚫

+FILEHTTPSTAT: <mode>,<stat>[,<length>]<CR><LF>

+FILEHTTPSTAT: <stat>,<errcode><CR><LF>

Unsolicited report

Timeout

N/A.

Parameter

**<mode>**

HTTP(S) request type.

0: HTTP(S) GET

1: HTTP(S) POST

Copyright © Neoway Technology Co., Ltd

136





N58 AT Commands Manual

Chapter9 HTTP/HTTPS Commands

**<stat>**

Download/upload result code

0: download/ upload failed

1: download/ upload successful

Length of the download/upload file, unit: byte.

Refer to Appendix A.

**<length>**

**<errcode>**

Example

AT+FILEHTTPSACTION=0,0,524288

OK

Download 512 KB data, starting from the first

byte.

The file with 512KB data length is posted

successfully.

+FILEHTTPSTAT: 0,1,524288

AT+FILEHTTPSACTION=1,524288,0,text.txt

OK

Adopt x-www-form-urlencoded type to post

text.txt file.

The file with 524288 data length is posted

successfully.

+FILEHTTPSTAT: 1,1,524288

9.22 +HTTPSCLOSED–HTTPS Link Closed Report

Unsolicited report that an HTTPS link is closed.

Format

Type

Command

Unsolicited report

+HTTPSCLOSED: HTTPS Link Closed

Timeout

N/A.

Parameter

N/A.

Example

+HTTPSCLOSED: HTTPS Link Closed

Unsolicited report that the HTTPS link is closed.

Copyright © Neoway Technology Co., Ltd

137

