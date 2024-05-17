int UDPSocketLibevent::SendTo(IOBuffer* buf,
                              int buf_len,
                              const IPEndPoint& address,
                              const CompletionCallback& callback) {
  return SendToOrWrite(buf, buf_len, &address, callback);
}
