int UDPSocketWin::Write(IOBuffer* buf,
                        int buf_len,
                        const CompletionCallback& callback) {
  return SendToOrWrite(buf, buf_len, NULL, callback);
}
