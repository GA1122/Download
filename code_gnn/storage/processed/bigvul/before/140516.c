bool HttpProxyClientSocket::GetSSLInfo(SSLInfo* ssl_info) {
  if (transport_.get() && transport_->socket()) {
    return transport_->socket()->GetSSLInfo(ssl_info);
  }
  NOTREACHED();
  return false;
}
