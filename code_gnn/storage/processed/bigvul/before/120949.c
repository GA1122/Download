int SocketStream::DoSecureProxyConnectComplete(int result) {
  DCHECK_EQ(STATE_NONE, next_state_);
  if (result == ERR_SSL_CLIENT_AUTH_CERT_NEEDED)
    return HandleCertificateRequest(result, &proxy_ssl_config_);

  if (IsCertificateError(result))
    next_state_ = STATE_SECURE_PROXY_HANDLE_CERT_ERROR;
  else if (result == OK)
    next_state_ = STATE_GENERATE_PROXY_AUTH_TOKEN;
  else
    next_state_ = STATE_CLOSE;
  return result;
}
