int SocketStream::DidEstablishConnection() {
  if (!socket_.get() || !socket_->IsConnected()) {
    next_state_ = STATE_CLOSE;
    return ERR_CONNECTION_FAILED;
  }
  next_state_ = STATE_READ_WRITE;
  metrics_->OnConnected();

  net_log_.EndEvent(NetLog::TYPE_SOCKET_STREAM_CONNECT);
  if (delegate_)
    delegate_->OnConnected(this, max_pending_send_allowed_);

  return OK;
}
