void WebContext::setCookiePolicy(CookiePolicy policy) {
  if (IsInitialized()) {
    context_->SetCookiePolicy(
        static_cast<net::StaticCookiePolicy::Type>(policy));
  } else {
    construct_props_->cookie_policy =
        static_cast<net::StaticCookiePolicy::Type>(policy);
  }
}
