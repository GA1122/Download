void WebContext::BrowserContextDelegate::SetAllowedExtraURLSchemes(
    const std::set<std::string>& schemes) {
  base::AutoLock lock(url_schemes_lock_);
  allowed_extra_url_schemes_ = schemes;
}
