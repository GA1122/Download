bool DownloadItemImpl::IsPaused() const {
  return job_ ? job_->is_paused() : false;
}
