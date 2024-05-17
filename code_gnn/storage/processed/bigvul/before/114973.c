void TestingAutomationProvider::IsBrowserInApplicationMode(int handle,
                                                           bool* is_application,
                                                           bool* success) {
  *is_application = false;
  *success = false;

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    if (browser) {
      *success = true;
      *is_application = browser->is_app();
    }
  }
}
