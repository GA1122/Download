void OfflineLoadPage::DontProceed() {
  if (proceeded_)
    return;
  delegate_->OnBlockingPageComplete(false);
  InterstitialPage::DontProceed();
}
