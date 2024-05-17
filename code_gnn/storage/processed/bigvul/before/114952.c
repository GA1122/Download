void TestingAutomationProvider::GetTabCount(int handle, int* tab_count) {
  *tab_count = -1;   

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    *tab_count = browser->tab_count();
  }
}
