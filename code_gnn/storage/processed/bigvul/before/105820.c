void UrlmonUrlRequestManager::GetCookiesForUrl(const GURL& url, int cookie_id) {
  DWORD cookie_size = 0;
  bool success = true;
  std::string cookie_string;

  int32 cookie_action = COOKIEACTION_READ;
  BOOL result = InternetGetCookieA(url.spec().c_str(), NULL, NULL,
                                   &cookie_size);
  DWORD error = 0;
  if (cookie_size) {
    scoped_array<char> cookies(new char[cookie_size + 1]);
    if (!InternetGetCookieA(url.spec().c_str(), NULL, cookies.get(),
                            &cookie_size)) {
      success = false;
      error = GetLastError();
      NOTREACHED() << "InternetGetCookie failed. Error: " << error;
    } else {
      cookie_string = cookies.get();
    }
  } else {
    success = false;
    error = GetLastError();
    DVLOG(1) << "InternetGetCookie failed. Error: " << error;
  }

  OnCookiesRetrieved(success, url, cookie_string, cookie_id);
  if (!success && !error)
    cookie_action = COOKIEACTION_SUPPRESS;

  AddPrivacyDataForUrl(url.spec(), "", cookie_action);
}
