static int GetCertID(net::URLRequest* request, int child_id) {
  if (request->ssl_info().cert) {
    return CertStore::GetInstance()->StoreCert(request->ssl_info().cert,
                                               child_id);
  }
  return 0;
}
