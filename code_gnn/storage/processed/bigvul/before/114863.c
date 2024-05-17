void TestingAutomationProvider::CloseBrowserAsync(int browser_handle) {
  if (!browser_tracker_->ContainsHandle(browser_handle))
    return;

  Browser* browser = browser_tracker_->GetResource(browser_handle);
  browser->window()->Close();
}
