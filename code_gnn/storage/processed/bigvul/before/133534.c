void WebContentsImpl::ActivateAndShowRepostFormWarningDialog() {
  Activate();
  if (delegate_)
    delegate_->ShowRepostFormWarningDialog(this);
}
