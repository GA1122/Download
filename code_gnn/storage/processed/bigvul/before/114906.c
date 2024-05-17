void TestingAutomationProvider::GetCookies(const GURL& url, int handle,
                                           int* value_size,
                                           std::string* value) {
  WebContents* contents = tab_tracker_->ContainsHandle(handle) ?
      tab_tracker_->GetResource(handle)->GetWebContents() : NULL;
  automation_util::GetCookies(url, contents, value_size, value);
}
