int SSLClientSocketOpenSSL::DoVerifyCert(int result) {
  DCHECK(server_cert_.get());
  GotoState(STATE_VERIFY_CERT_COMPLETE);

  CertStatus cert_status;
  if (ssl_config_.IsAllowedBadCert(server_cert_.get(), &cert_status)) {
    VLOG(1) << "Received an expected bad cert with status: " << cert_status;
    server_cert_verify_result_.Reset();
    server_cert_verify_result_.cert_status = cert_status;
    server_cert_verify_result_.verified_cert = server_cert_;
    return OK;
  }

  int flags = 0;
  if (ssl_config_.rev_checking_enabled)
    flags |= CertVerifier::VERIFY_REV_CHECKING_ENABLED;
  if (ssl_config_.verify_ev_cert)
    flags |= CertVerifier::VERIFY_EV_CERT;
  if (ssl_config_.cert_io_enabled)
    flags |= CertVerifier::VERIFY_CERT_IO_ENABLED;
  if (ssl_config_.rev_checking_required_local_anchors)
    flags |= CertVerifier::VERIFY_REV_CHECKING_REQUIRED_LOCAL_ANCHORS;
  verifier_.reset(new SingleRequestCertVerifier(cert_verifier_));
  return verifier_->Verify(
      server_cert_.get(),
      host_and_port_.host(),
      flags,
      NULL  ,
      &server_cert_verify_result_,
      base::Bind(&SSLClientSocketOpenSSL::OnHandshakeIOComplete,
                 base::Unretained(this)),
      net_log_);
}
