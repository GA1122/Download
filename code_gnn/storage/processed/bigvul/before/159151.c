WebContents* DownloadItemImpl::GetWebContents() const {
  if (job_)
    return job_->GetWebContents();
  return nullptr;
}
