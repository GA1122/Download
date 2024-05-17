int SocketStream::DoSecureProxyHandleCertError(int result) {
  DCHECK_EQ(STATE_NONE, next_state_);
  DCHECK(IsCertificateError(result));
  result = HandleCertificateError(result);
  if (result == ERR_IO_PENDING)
    next_state_ = STATE_SECURE_PROXY_HANDLE_CERT_ERROR_COMPLETE;
  else
    next_state_ = STATE_CLOSE;
  return result;
}
