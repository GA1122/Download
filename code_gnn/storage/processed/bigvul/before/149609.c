void PreconnectManager::Stop(const GURL& url) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto it = preresolve_info_.find(url.host());
  if (it == preresolve_info_.end()) {
    return;
  }

  it->second->was_canceled = true;
}
