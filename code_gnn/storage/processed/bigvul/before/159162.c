bool DownloadItemImpl::IsSavePackageDownload() const {
  return job_ && job_->IsSavePackageDownload();
}
