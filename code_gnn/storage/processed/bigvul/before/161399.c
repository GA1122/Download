Response SecurityHandler::SetOverrideCertificateErrors(bool override) {
  if (override) {
    if (!enabled_)
      return Response::Error("Security domain not enabled");
    if (cert_error_override_mode_ == CertErrorOverrideMode::kIgnoreAll)
      return Response::Error("Certificate errors are already being ignored.");
    cert_error_override_mode_ = CertErrorOverrideMode::kHandleEvents;
  } else {
    cert_error_override_mode_ = CertErrorOverrideMode::kDisabled;
    FlushPendingCertificateErrorNotifications();
  }
  return Response::OK();
}
