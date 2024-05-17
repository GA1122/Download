void WebContentsImpl::CancelModalDialogsForRenderManager() {
  if (dialog_manager_)
    dialog_manager_->CancelActiveAndPendingDialogs(this);
}
