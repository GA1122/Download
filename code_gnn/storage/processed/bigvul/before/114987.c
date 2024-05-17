void TestingAutomationProvider::NavigationAsyncWithDisposition(
    int handle,
    const GURL& url,
    WindowOpenDisposition disposition,
    bool* status) {
  *status = false;

  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);

    Browser* browser = FindAndActivateTab(tab);

    if (browser) {
      OpenURLParams params(
          url, Referrer(), disposition, content::PAGE_TRANSITION_TYPED, false);
      browser->OpenURL(params);
      *status = true;
    }
  }
}
