int UDPSocketLibevent::InternalRecvFrom(IOBuffer* buf, int buf_len,
                                        IPEndPoint* address) {
  int bytes_transferred;
  int flags = 0;

  SockaddrStorage storage;

  bytes_transferred =
      HANDLE_EINTR(recvfrom(socket_,
                            buf->data(),
                            buf_len,
                            flags,
                            storage.addr,
                            &storage.addr_len));
  int result;
  if (bytes_transferred >= 0) {
    result = bytes_transferred;
    if (address && !address->FromSockAddr(storage.addr, storage.addr_len))
      result = ERR_FAILED;
  } else {
    result = MapSystemError(errno);
  }
  if (result != ERR_IO_PENDING)
    LogRead(result, buf->data(), storage.addr_len, storage.addr);
  return result;
}
