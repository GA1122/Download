void TestingAutomationProvider::GetParentBrowserOfTab(int tab_handle,
                                                      int* browser_handle,
                                                      bool* success) {
  *success = false;
  if (tab_tracker_->ContainsHandle(tab_handle)) {
    NavigationController* controller = tab_tracker_->GetResource(tab_handle);
    int index;
    Browser* browser = Browser::GetBrowserForController(controller, &index);
    if (browser) {
      *browser_handle = browser_tracker_->Add(browser);
      *success = true;
    }
  }
}
