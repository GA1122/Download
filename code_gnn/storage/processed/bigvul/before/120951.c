int SocketStream::DoSecureProxyHandleCertErrorComplete(int result) {
  DCHECK_EQ(STATE_NONE, next_state_);
  if (result == OK) {
    if (!socket_->IsConnectedAndIdle())
      return AllowCertErrorForReconnection(&proxy_ssl_config_);
    next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
  } else {
    next_state_ = STATE_CLOSE;
  }
  return result;
}
