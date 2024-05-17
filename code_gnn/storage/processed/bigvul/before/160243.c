  void NavigateToPageAndWaitForReadyTitle(const std::string path) {
    const base::string16 expected_title1 = base::ASCIIToUTF16("READY");
    content::TitleWatcher title_watcher1(
        browser()->tab_strip_model()->GetActiveWebContents(), expected_title1);
    ui_test_utils::NavigateToURL(browser(),
                                 embedded_test_server()->GetURL(path));
    EXPECT_EQ(expected_title1, title_watcher1.WaitAndGetTitle());
  }
