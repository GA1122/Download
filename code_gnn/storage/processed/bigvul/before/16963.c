bool WebContext::BrowserContextDelegate::IsCustomProtocolHandlerRegistered(
    const std::string& scheme) const {
  base::AutoLock lock(url_schemes_lock_);
  return allowed_extra_url_schemes_.find(scheme) !=
      allowed_extra_url_schemes_.end();
}
