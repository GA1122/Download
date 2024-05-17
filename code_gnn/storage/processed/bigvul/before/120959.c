int SocketStream::HandleCertificateRequest(int result, SSLConfig* ssl_config) {
  if (ssl_config->send_client_cert) {
    return result;
  }

  DCHECK(socket_.get());
  scoped_refptr<SSLCertRequestInfo> cert_request_info = new SSLCertRequestInfo;
  SSLClientSocket* ssl_socket =
      static_cast<SSLClientSocket*>(socket_.get());
  ssl_socket->GetSSLCertRequestInfo(cert_request_info.get());

  HttpTransactionFactory* factory = context_->http_transaction_factory();
  if (!factory)
    return result;
  scoped_refptr<HttpNetworkSession> session = factory->GetSession();
  if (!session.get())
    return result;

  scoped_refptr<X509Certificate> client_cert;
  if (!session->ssl_client_auth_cache()->Lookup(
          cert_request_info->host_and_port, &client_cert)) {
    return result;
  }

  const std::vector<std::string>& cert_authorities =
      cert_request_info->cert_authorities;
  if (client_cert.get() && !cert_authorities.empty() &&
      !client_cert->IsIssuedByEncoded(cert_authorities)) {
    return result;
  }

  ssl_config->send_client_cert = true;
  ssl_config->client_cert = client_cert;
  next_state_ = STATE_TCP_CONNECT;
  return OK;
}
