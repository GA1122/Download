const GURL& DownloadItemImpl::GetOriginalUrl() const {
  return url_chain_.front();
}
