  virtual void SetUp() {
    ChromeRenderViewHostTestHarness::SetUp();
    SafeBrowsingBlockingPage::RegisterFactory(&factory_);
    ResetUserResponse();
  }
