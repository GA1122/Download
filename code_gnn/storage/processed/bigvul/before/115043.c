void TestingAutomationProvider::ShutdownSessionService(int handle,
                                                       bool* result) {
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    SessionServiceFactory::ShutdownForProfile(browser->profile());
    *result = true;
  } else {
    *result = false;
  }
}
