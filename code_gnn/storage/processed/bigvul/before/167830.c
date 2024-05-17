void DownloadRequestLimiter::TabDownloadState::WebContentsDestroyed() {

  NotifyCallbacks(false);
  host_->Remove(this, web_contents());
}
