void ResourceDispatcherHost::OnSSLCertificateError(
    URLRequest* request,
    int cert_error,
    net::X509Certificate* cert) {
  DCHECK(request);
  SSLManager::OnSSLCertificateError(this, request, cert_error, cert);
}
