  void InitializeServiceWorkerFetchTestPage() {
    const base::string16 expected_title = base::ASCIIToUTF16("READY");
    content::TitleWatcher title_watcher(
        browser()->tab_strip_model()->GetActiveWebContents(), expected_title);
    ui_test_utils::NavigateToURL(browser(),
                                 embedded_test_server()->GetURL("/test.html"));
    EXPECT_EQ(expected_title, title_watcher.WaitAndGetTitle());
  }
