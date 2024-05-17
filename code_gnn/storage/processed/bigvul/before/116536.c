  static TestSiteInstance* CreateTestSiteInstance(
      BrowserContext* browser_context,
      int* site_delete_counter,
      int* browsing_delete_counter) {
    TestBrowsingInstance* browsing_instance =
        new TestBrowsingInstance(browser_context, browsing_delete_counter);
    return new TestSiteInstance(browsing_instance, site_delete_counter);
  }
