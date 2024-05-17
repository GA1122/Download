void WebContentsImpl::CancelActiveAndPendingDialogs() {
  if (dialog_manager_)
    dialog_manager_->CancelActiveAndPendingDialogs(this);
  if (browser_plugin_embedder_)
    browser_plugin_embedder_->CancelGuestDialogs();
}
