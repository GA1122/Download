int UDPSocketLibevent::Connect(const IPEndPoint& address) {
  net_log_.BeginEvent(NetLog::TYPE_UDP_CONNECT,
                      CreateNetLogUDPConnectCallback(&address));
  int rv = InternalConnect(address);
  if (rv != OK)
    Close();
  net_log_.EndEventWithNetErrorCode(NetLog::TYPE_UDP_CONNECT, rv);
  return rv;
}
