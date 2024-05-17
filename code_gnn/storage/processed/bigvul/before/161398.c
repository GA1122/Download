Response SecurityHandler::SetIgnoreCertificateErrors(bool ignore) {
  if (ignore) {
    if (cert_error_override_mode_ == CertErrorOverrideMode::kHandleEvents)
      return Response::Error("Certificate errors are already overridden.");
    cert_error_override_mode_ = CertErrorOverrideMode::kIgnoreAll;
  } else {
    cert_error_override_mode_ = CertErrorOverrideMode::kDisabled;
  }
  return Response::OK();
}
