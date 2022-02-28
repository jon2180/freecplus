#pragma once

namespace freecplus {

// 以下是socket通信的函数和类

// socket通信的客户端类
class TcpClient {
public:
  int m_sockfd;    // 客户端的socket.
  char m_ip[21];   // 服务端的ip地址。
  int m_port;      // 与服务端通信的端口。
  bool m_btimeout; // 调用Read和Write方法时，失败的原因是否是超时：true-未超时，false-已超时。
  int m_buflen;    // 调用Read方法后，接收到的报文的大小，单位：字节。

  TcpClient(); // 构造函数。

  // 向服务端发起连接请求。
  // ip：服务端的ip地址。
  // port：服务端监听的端口。
  // 返回值：true-成功；false-失败。
  bool ConnectToServer(const char *ip, int port);

  // 接收服务端发送过来的数据。
  // buffer：接收数据缓冲区的地址，数据的长度存放在m_buflen成员变量中。
  // itimeout：等待数据的超时时间，单位：秒，缺省值是0-无限等待。
  // 返回值：true-成功；false-失败，失败有两种情况：1）等待超时，成员变量m_btimeout的值被设置为true；2）socket连接已不可用。
  bool Read(char *buffer, int itimeout = 0);

  // 向服务端发送数据。
  // buffer：待发送数据缓冲区的地址。
  // ibuflen：待发送数据的大小，单位：字节，缺省值为0，如果发送的是ascii字符串，ibuflen取0，如果是二进制流数据，ibuflen为二进制数据块的大小。
  // 返回值：true-成功；false-失败，如果失败，表示socket连接已不可用。
  bool Write(const char *buffer, int ibuflen = 0);

  // 断开与服务端的连接
  void Close();

  ~TcpClient(); // 析构函数自动关闭socket，释放资源。
};

} // namespace freecplus