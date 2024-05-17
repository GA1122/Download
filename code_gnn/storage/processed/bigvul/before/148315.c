void WebContentsImpl::CancelActiveAndPendingDialogs() {
  if (dialog_manager_) {
    dialog_manager_->CancelDialogs(this,  false);
  }
  if (browser_plugin_embedder_)
    browser_plugin_embedder_->CancelGuestDialogs();
}
