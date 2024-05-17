void HttpProxyClientSocket::Disconnect() {
  if (transport_.get())
    transport_->socket()->Disconnect();

  next_state_ = STATE_NONE;
  user_callback_.Reset();
}
