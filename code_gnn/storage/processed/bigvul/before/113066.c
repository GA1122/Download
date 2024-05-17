WebContents* DownloadItemImpl::GetWebContents() const {
  if (request_handle_.get())
    return request_handle_->GetWebContents();
  return NULL;
}
