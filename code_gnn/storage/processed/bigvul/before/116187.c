void SSLCertErrorHandler::OnDispatchFailed() {
  if (net::IsCertStatusMinorError(ssl_info().cert_status)) {
    ContinueRequest();
  } else {
    CancelRequest();
  }
}
