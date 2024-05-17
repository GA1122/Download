  TestBrowsingInstance(BrowserContext* browser_context, int* delete_counter)
      : BrowsingInstance(browser_context),
        use_process_per_site_(false),
        delete_counter_(delete_counter) {
  }
