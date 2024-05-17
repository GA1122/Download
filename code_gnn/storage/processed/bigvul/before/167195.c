bool GlobalConfirmInfoBar::DelegateProxy::Accept() {
  base::WeakPtr<GlobalConfirmInfoBar> info_bar = global_info_bar_;
  if (info_bar) {
    info_bar->OnInfoBarRemoved(info_bar_, false);
    info_bar->delegate_->Accept();
  }
  if (info_bar)
      info_bar->Close();
  return true;
}
