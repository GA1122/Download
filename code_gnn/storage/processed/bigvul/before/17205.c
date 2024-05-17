net::CookieStore* BrowserContextIOData::GetCookieStore() const {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  return cookie_store_owner_->store();
}
