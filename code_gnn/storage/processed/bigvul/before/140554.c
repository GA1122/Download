bool SpdyProxyClientSocket::IsConnected() const {
  return next_state_ == STATE_OPEN;
}
