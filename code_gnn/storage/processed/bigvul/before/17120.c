void OxideQQuickWebViewPrivate::killWebProcess(bool crash) {
  if (!proxy_) {
    return;
  }

  proxy_->killWebProcess(crash);
}
