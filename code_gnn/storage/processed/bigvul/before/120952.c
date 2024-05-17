int SocketStream::DoTcpConnect(int result) {
  if (result != OK) {
    next_state_ = STATE_CLOSE;
    return result;
  }
  next_state_ = STATE_TCP_CONNECT_COMPLETE;
  DCHECK(factory_);
  socket_.reset(factory_->CreateTransportClientSocket(addresses_,
                                                      net_log_.net_log(),
                                                      net_log_.source()));
  metrics_->OnStartConnection();
  return socket_->Connect(io_callback_);
}
