bool SecurityHandler::NotifyCertificateError(int cert_error,
                                             const GURL& request_url,
                                             CertErrorCallback handler) {
  if (cert_error_override_mode_ == CertErrorOverrideMode::kIgnoreAll) {
    if (handler)
      handler.Run(content::CERTIFICATE_REQUEST_RESULT_TYPE_CONTINUE);
    return true;
  }

  if (!enabled_)
    return false;

  frontend_->CertificateError(++last_cert_error_id_,
                              net::ErrorToShortString(cert_error),
                              request_url.spec());

  if (!handler ||
      cert_error_override_mode_ != CertErrorOverrideMode::kHandleEvents) {
    return false;
  }

  cert_error_callbacks_[last_cert_error_id_] = handler;
  return true;
}
