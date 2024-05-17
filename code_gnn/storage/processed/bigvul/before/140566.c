int SpdyProxyClientSocket::RestartWithAuth(const CompletionCallback& callback) {
  next_state_ = STATE_DISCONNECTED;
  return OK;
}
