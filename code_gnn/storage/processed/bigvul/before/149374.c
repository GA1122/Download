content::BrowserContext* CheckClientDownloadRequest::GetBrowserContext() {
  return content::DownloadItemUtils::GetBrowserContext(item_);
}
