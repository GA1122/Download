void SpdyProxyClientSocket::OnIOComplete(int result) {
  DCHECK_NE(STATE_DISCONNECTED, next_state_);
  int rv = DoLoop(result);
  if (rv != ERR_IO_PENDING) {
    CompletionCallback c = read_callback_;
    read_callback_.Reset();
    c.Run(rv);
  }
}
