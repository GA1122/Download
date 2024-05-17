void UrlmonUrlRequestManager::OnCookiesRetrieved(bool success, const GURL& url,
    const std::string& cookie_string, int cookie_id) {
  DCHECK(url.is_valid());
  delegate_->OnCookiesRetrieved(success, url, cookie_string, cookie_id);
}
