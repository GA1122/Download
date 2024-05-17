SpdyProxyClientSocket::~SpdyProxyClientSocket() {
  Disconnect();
  net_log_.EndEvent(NetLog::TYPE_SOCKET_ALIVE);
}
