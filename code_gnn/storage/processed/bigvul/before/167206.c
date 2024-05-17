void GlobalConfirmInfoBar::DelegateProxy::InfoBarDismissed() {
  base::WeakPtr<GlobalConfirmInfoBar> info_bar = global_info_bar_;
  if (info_bar) {
    info_bar->OnInfoBarRemoved(info_bar_, false);
    info_bar->delegate_->InfoBarDismissed();
  }
  if (info_bar)
      info_bar->Close();
}
