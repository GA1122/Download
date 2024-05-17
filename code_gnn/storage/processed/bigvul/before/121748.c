int UDPSocketLibevent::CreateSocket(int addr_family) {
  addr_family_ = addr_family;
  socket_ = CreatePlatformSocket(addr_family_, SOCK_DGRAM, 0);
  if (socket_ == kInvalidSocket)
    return MapSystemError(errno);
  if (SetNonBlocking(socket_)) {
    const int err = MapSystemError(errno);
    Close();
    return err;
  }
  return OK;
}
