void GaiaCookieManagerService::Shutdown() {
  cookie_changed_subscription_.reset();
}
