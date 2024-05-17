int HttpProxyClientSocket::DoTCPRestart() {
  next_state_ = STATE_TCP_RESTART_COMPLETE;
  return transport_->socket()->Connect(
      base::Bind(&HttpProxyClientSocket::OnIOComplete, base::Unretained(this)));
}
