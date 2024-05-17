int SocketStream::DoSOCKSConnectComplete(int result) {
  DCHECK_EQ(kSOCKSProxy, proxy_mode_);

  if (result == OK) {
    if (is_secure())
      next_state_ = STATE_SSL_CONNECT;
    else
      result = DidEstablishConnection();
  } else {
    next_state_ = STATE_CLOSE;
  }
  return result;
}
