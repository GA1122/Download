void OfflineLoadPage::Proceed() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  proceeded_ = true;
  delegate_->OnBlockingPageComplete(true);
  InterstitialPage::Proceed();
}
