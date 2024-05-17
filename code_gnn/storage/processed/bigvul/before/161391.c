void SecurityHandler::FlushPendingCertificateErrorNotifications() {
  for (auto callback : cert_error_callbacks_)
    callback.second.Run(content::CERTIFICATE_REQUEST_RESULT_TYPE_CANCEL);
  cert_error_callbacks_.clear();
}
