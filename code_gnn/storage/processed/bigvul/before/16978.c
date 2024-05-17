WebContextProxy::CookiePolicy WebContext::cookiePolicy() const {
  if (IsInitialized()) {
    return static_cast<CookiePolicy>(context_->GetCookiePolicy());
  }

  return static_cast<CookiePolicy>(construct_props_->cookie_policy);
}
