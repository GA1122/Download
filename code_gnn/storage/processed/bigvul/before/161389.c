Response SecurityHandler::Disable() {
  enabled_ = false;
  cert_error_override_mode_ = CertErrorOverrideMode::kDisabled;
  WebContentsObserver::Observe(nullptr);
  FlushPendingCertificateErrorNotifications();
  return Response::OK();
}
