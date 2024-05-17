UDPSocketWin::Core::Core(UDPSocketWin* socket)
    : socket_(socket),
      reader_(this),
      writer_(this) {
  memset(&read_overlapped_, 0, sizeof(read_overlapped_));
  memset(&write_overlapped_, 0, sizeof(write_overlapped_));

  read_overlapped_.hEvent = WSACreateEvent();
  write_overlapped_.hEvent = WSACreateEvent();
}
