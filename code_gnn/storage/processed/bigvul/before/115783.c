  virtual SafeBrowsingBlockingPage* CreateSafeBrowsingPage(
      SafeBrowsingService* service,
      TabContents* tab_contents,
      const SafeBrowsingBlockingPage::UnsafeResourceList& unsafe_resources) {
    return new TestSafeBrowsingBlockingPage(service, tab_contents,
                                            unsafe_resources);
  }
