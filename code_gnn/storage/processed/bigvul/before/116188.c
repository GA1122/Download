void SSLCertErrorHandler::OnDispatched() {
  manager_->policy()->OnCertError(this);
}
