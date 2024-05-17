int UDPSocketWin::InternalRecvFrom(IOBuffer* buf, int buf_len,
                                   IPEndPoint* address) {
  DCHECK(!core_->read_iobuffer_);
  SockaddrStorage& storage = core_->recv_addr_storage_;
  storage.addr_len = sizeof(storage.addr_storage);

  WSABUF read_buffer;
  read_buffer.buf = buf->data();
  read_buffer.len = buf_len;

  DWORD flags = 0;
  DWORD num;
  CHECK_NE(INVALID_SOCKET, socket_);
  AssertEventNotSignaled(core_->read_overlapped_.hEvent);
  int rv = WSARecvFrom(socket_, &read_buffer, 1, &num, &flags, storage.addr,
                       &storage.addr_len, &core_->read_overlapped_, NULL);
  if (rv == 0) {
    if (ResetEventIfSignaled(core_->read_overlapped_.hEvent)) {
      int result = num;
      if (address && result >= 0) {
        if (!ReceiveAddressToIPEndpoint(address))
          result = ERR_FAILED;
      }
      LogRead(result, buf->data());
      return result;
    }
  } else {
    int os_error = WSAGetLastError();
    if (os_error != WSA_IO_PENDING) {
      int result = MapSystemError(os_error);
      LogRead(result, NULL);
      return result;
    }
  }
  core_->WatchForRead();
  core_->read_iobuffer_ = buf;
  return ERR_IO_PENDING;
}
