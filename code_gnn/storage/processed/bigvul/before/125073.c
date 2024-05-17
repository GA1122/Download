  virtual void SetUp() {
    old_browser_client_ = GetContentClient()->browser();
    GetContentClient()->set_browser_for_testing(&browser_client_);
    url_util::AddStandardScheme(kPrivilegedScheme);
    url_util::AddStandardScheme(chrome::kChromeUIScheme);
  }
