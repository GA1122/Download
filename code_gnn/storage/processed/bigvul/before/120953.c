int SocketStream::DoTcpConnectComplete(int result) {
  if (result != OK) {
    next_state_ = STATE_CLOSE;
    return result;
  }

  if (proxy_mode_ == kTunnelProxy) {
    if (proxy_info_.is_https())
      next_state_ = STATE_SECURE_PROXY_CONNECT;
    else
      next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
  } else if (proxy_mode_ == kSOCKSProxy) {
    next_state_ = STATE_SOCKS_CONNECT;
  } else if (is_secure()) {
    next_state_ = STATE_SSL_CONNECT;
  } else {
    result = DidEstablishConnection();
  }
  return result;
}
