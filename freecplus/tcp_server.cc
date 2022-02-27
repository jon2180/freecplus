#include "tcp_server.h"

#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "util/socket_util.h"

CTcpServer::CTcpServer() {
  m_listenfd = -1;
  m_connfd = -1;
  m_socklen = 0;
  m_btimeout = false;
}

bool CTcpServer::InitServer(const unsigned int port) {
  if (m_listenfd > 0) {
    close(m_listenfd);
    m_listenfd = -1;
  }

  m_listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // WINDOWS平台如下
  // char b_opt='1';
  // setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&b_opt,sizeof(b_opt));

  // Linux如下
  int opt = 1;
  unsigned int len = sizeof(opt);
  setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);

  memset(&m_servaddr, 0, sizeof(m_servaddr));
  m_servaddr.sin_family = AF_INET;
  m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  m_servaddr.sin_port = htons(port);
  if (bind(m_listenfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) != 0) {
    CloseListen();
    return false;
  }

  if (listen(m_listenfd, 5) != 0) {
    CloseListen();
    return false;
  }

  m_socklen = sizeof(struct sockaddr_in);

  return true;
}

bool CTcpServer::Accept() {
  if (m_listenfd == -1)
    return false;

  if ((m_connfd = accept(m_listenfd, (struct sockaddr *)&m_clientaddr, (socklen_t *)&m_socklen)) < 0)
    return false;

  return true;
}

char *CTcpServer::GetIP() { return (inet_ntoa(m_clientaddr.sin_addr)); }

bool CTcpServer::Read(char *buffer, const int itimeout) {
  if (m_connfd == -1)
    return false;

  if (itimeout > 0) {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(m_connfd, &tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout;
    timeout.tv_usec = 0;

    m_btimeout = false;

    int i;
    if ((i = select(m_connfd + 1, &tmpfd, 0, 0, &timeout)) <= 0) {
      if (i == 0)
        m_btimeout = true;
      return false;
    }
  }

  m_buflen = 0;
  return (TcpRead(m_connfd, buffer, &m_buflen));
}

bool CTcpServer::Write(const char *buffer, const int ibuflen) {
  if (m_connfd == -1)
    return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_connfd, &tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  m_btimeout = false;

  int i;
  if ((i = select(m_connfd + 1, 0, &tmpfd, 0, &timeout)) <= 0) {
    if (i == 0)
      m_btimeout = true;
    return false;
  }

  int ilen = ibuflen;
  if (ilen == 0)
    ilen = strlen(buffer);

  return (TcpWrite(m_connfd, buffer, ilen));
}

void CTcpServer::CloseListen() {
  if (m_listenfd > 0) {
    close(m_listenfd);
    m_listenfd = -1;
  }
}

void CTcpServer::CloseClient() {
  if (m_connfd > 0) {
    close(m_connfd);
    m_connfd = -1;
  }
}

CTcpServer::~CTcpServer() {
  CloseListen();
  CloseClient();
}
