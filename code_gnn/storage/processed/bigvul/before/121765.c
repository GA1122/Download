int UDPSocketLibevent::Read(IOBuffer* buf,
                            int buf_len,
                            const CompletionCallback& callback) {
  return RecvFrom(buf, buf_len, NULL, callback);
}
