void TestingAutomationProvider::ShowCollectedCookiesDialog(
    int handle, bool* success) {
  *success = false;
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* controller = tab_tracker_->GetResource(handle);
    WebContents* tab_contents = controller->GetWebContents();
    Browser* browser = Browser::GetBrowserForController(controller, NULL);
    browser->ShowCollectedCookiesDialog(
        TabContentsWrapper::GetCurrentWrapperForContents(tab_contents));
    *success = true;
  }
}
