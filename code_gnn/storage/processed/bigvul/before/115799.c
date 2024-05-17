  virtual void SetUp() {
    SafeBrowsingService::RegisterFactory(&factory_);
    SafeBrowsingBlockingPage::RegisterFactory(&blocking_page_factory_);
    MalwareDetails::RegisterFactory(&details_factory_);
    InProcessBrowserTest::SetUp();
  }
