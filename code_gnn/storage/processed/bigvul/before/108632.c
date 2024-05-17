  virtual void OnCertificateRequested(
      URLRequest* request,
      SSLCertRequestInfo* cert_request_info) {
    on_certificate_requested_count_++;
    MessageLoop::current()->Quit();
  }
