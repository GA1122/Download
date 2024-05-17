UDPSocketLibevent::~UDPSocketLibevent() {
  Close();
  net_log_.EndEvent(NetLog::TYPE_SOCKET_ALIVE);
}
