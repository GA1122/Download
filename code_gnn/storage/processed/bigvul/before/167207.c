bool GlobalConfirmInfoBar::DelegateProxy::LinkClicked(
    WindowOpenDisposition disposition) {
  return global_info_bar_ ?
      global_info_bar_->delegate_->LinkClicked(disposition) : false;
}
