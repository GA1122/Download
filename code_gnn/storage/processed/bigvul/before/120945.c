int SocketStream::DoSSLConnectComplete(int result) {
  DCHECK_EQ(STATE_NONE, next_state_);
  if (result == ERR_SSL_CLIENT_AUTH_CERT_NEEDED)
    return HandleCertificateRequest(result, &server_ssl_config_);

  if (IsCertificateError(result))
    next_state_ = STATE_SSL_HANDLE_CERT_ERROR;
  else if (result == OK)
    result = DidEstablishConnection();
  else
    next_state_ = STATE_CLOSE;
  return result;
}
