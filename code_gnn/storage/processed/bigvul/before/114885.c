void TestingAutomationProvider::FindTabbedBrowserWindow(int* handle) {
  *handle = 0;
  Browser* browser = BrowserList::FindTabbedBrowser(profile_, false);
  if (browser)
    *handle = browser_tracker_->Add(browser);
}
