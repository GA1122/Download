void TestingAutomationProvider::BringBrowserToFront(int browser_handle,
                                                    bool* success) {
  *success = false;
  if (browser_tracker_->ContainsHandle(browser_handle)) {
    Browser* browser = browser_tracker_->GetResource(browser_handle);
    browser->window()->Activate();
    *success = true;
  }
}
