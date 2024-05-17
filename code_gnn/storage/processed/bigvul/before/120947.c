int SocketStream::DoSSLHandleCertErrorComplete(int result) {
  DCHECK_EQ(STATE_NONE, next_state_);

  if (result == OK) {
    if (!socket_->IsConnectedAndIdle())
      return AllowCertErrorForReconnection(&server_ssl_config_);
    result = DidEstablishConnection();
  } else {
    next_state_ = STATE_CLOSE;
  }
  return result;
}
