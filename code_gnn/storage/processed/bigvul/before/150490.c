void NavigationControllerImpl::SetPendingNavigationSSLError(bool error) {
  if (pending_entry_)
    pending_entry_->set_ssl_error(error);
}
