void DownloadItemImpl::TogglePause() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(IsInProgress());
  if (is_paused_)
    request_handle_->ResumeRequest();
  else
    request_handle_->PauseRequest();
  is_paused_ = !is_paused_;
  UpdateObservers();
}
