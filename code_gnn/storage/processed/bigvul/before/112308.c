bool ResourceDispatcherHostImpl::AcceptSSLClientCertificateRequest(
    ResourceLoader* loader,
    net::SSLCertRequestInfo* cert_info) {
  if (delegate_ && !delegate_->AcceptSSLClientCertificateRequest(
          loader->request(), cert_info)) {
    return false;
  }

  return true;
}
