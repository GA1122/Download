bool HttpProxyClientSocket::IsConnectedAndIdle() const {
  return next_state_ == STATE_DONE &&
    transport_->socket()->IsConnectedAndIdle();
}
