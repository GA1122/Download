void UrlmonUrlRequestManager::SetCookiesForUrl(const GURL& url,
                                               const std::string& cookie) {
  DCHECK(container_);
  if (container_) {
    container_->AddRef();
  }

  InternetCookieState cookie_state = static_cast<InternetCookieState>(
      InternetSetCookieExA(url.spec().c_str(), NULL, cookie.c_str(),
                           INTERNET_COOKIE_EVALUATE_P3P, NULL));

  int32 cookie_action = MapCookieStateToCookieAction(cookie_state);
  AddPrivacyDataForUrl(url.spec(), "", cookie_action);

  if (container_) {
    container_->Release();
  }
}
