int UDPSocketWin::Bind(const IPEndPoint& address) {
  DCHECK(!is_connected());
  int rv = CreateSocket(address.GetSockAddrFamily());
  if (rv < 0)
    return rv;
  rv = SetSocketOptions();
  if (rv < 0) {
    Close();
    return rv;
  }
  rv = DoBind(address);
  if (rv < 0) {
    Close();
    return rv;
  }
  local_address_.reset();
  return rv;
}
