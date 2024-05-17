int UDPSocketWin::CreateSocket(int addr_family) {
  addr_family_ = addr_family;
  socket_ = CreatePlatformSocket(addr_family_, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ == INVALID_SOCKET)
    return MapSystemError(WSAGetLastError());
  core_ = new Core(this);
  return OK;
}
