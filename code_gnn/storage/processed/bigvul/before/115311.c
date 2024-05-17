  void CloseWindowAndWait(Browser* browser) {
    size_t browser_count = BrowserList::size();
    ui_test_utils::WindowedNotificationObserver signal(
        chrome::NOTIFICATION_BROWSER_CLOSED,
        content::Source<Browser>(browser));
    browser->CloseWindow();
    signal.Wait();
    EXPECT_EQ(browser_count - 1, BrowserList::size());
  }
