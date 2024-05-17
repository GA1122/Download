bool HttpProxyClientSocket::IsConnected() const {
  return next_state_ == STATE_DONE && transport_->socket()->IsConnected();
}
