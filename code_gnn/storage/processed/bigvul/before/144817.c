Browser* CreateBrowserWithTabs(int num_tabs) {
  Browser* current_browser = BrowserList::GetInstance()->GetLastActive();
  ui_test_utils::BrowserAddedObserver browser_added_observer;
  chrome::NewWindow(current_browser);
  Browser* new_browser = browser_added_observer.WaitForSingleNewBrowser();
  EXPECT_EQ(new_browser, BrowserList::GetInstance()->GetLastActive());
  EnsureTabsInBrowser(new_browser, num_tabs);
  return new_browser;
}
