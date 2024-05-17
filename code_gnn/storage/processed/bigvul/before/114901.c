void TestingAutomationProvider::GetBrowserWindow(int index, int* handle) {
  *handle = 0;
  Browser* browser = automation_util::GetBrowserAt(index);
  if (browser)
    *handle = browser_tracker_->Add(browser);
}
