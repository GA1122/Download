bool ChromeClientImpl::CanOpenBeforeUnloadConfirmPanel() {
  return !!web_view_->Client();
}
