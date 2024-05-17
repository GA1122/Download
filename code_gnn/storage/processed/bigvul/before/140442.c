CaptureGroupNameSSLSocketPool::CaptureGroupNameSocketPool(
    HostResolver* host_resolver,
    CertVerifier* cert_verifier)
    : SSLClientSocketPool(0,
                          0,
                          NULL,
                          host_resolver,
                          cert_verifier,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          std::string(),
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          NULL,
                          false,
                          NULL) {
}
