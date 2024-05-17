net::StaticCookiePolicy::Type BrowserContextIOData::GetCookiePolicy() const {
  const BrowserContextSharedIOData& data = GetSharedData();
  base::AutoLock lock(data.lock);
  return data.cookie_policy;
}
