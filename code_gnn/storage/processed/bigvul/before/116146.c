void ResourceDispatcherHostImpl::OnCertificateRequested(
    net::URLRequest* request,
    net::SSLCertRequestInfo* cert_request_info) {
  DCHECK(request);
  if (delegate_ && !delegate_->AcceptSSLClientCertificateRequest(
          request, cert_request_info)) {
    request->Cancel();
    return;
  }

  if (cert_request_info->client_certs.empty()) {
    request->ContinueWithCertificate(NULL);
    return;
  }

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  DCHECK(!info->ssl_client_auth_handler()) <<
      "OnCertificateRequested called with ssl_client_auth_handler pending";
  info->set_ssl_client_auth_handler(
      new SSLClientAuthHandler(request, cert_request_info));
  info->ssl_client_auth_handler()->SelectCertificate();
}
