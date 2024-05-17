int SpdyProxyClientSocket::DoGenerateAuthToken() {
  next_state_ = STATE_GENERATE_AUTH_TOKEN_COMPLETE;
  return auth_->MaybeGenerateAuthToken(
      &request_,
      base::Bind(&SpdyProxyClientSocket::OnIOComplete,
                 weak_factory_.GetWeakPtr()),
      net_log_);
}
