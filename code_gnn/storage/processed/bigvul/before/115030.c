void TestingAutomationProvider::SetCookie(const GURL& url,
                                          const std::string& value,
                                          int handle,
                                          int* response_value) {
  WebContents* contents = tab_tracker_->ContainsHandle(handle) ?
      tab_tracker_->GetResource(handle)->GetWebContents() : NULL;
  automation_util::SetCookie(url, value, contents, response_value);
}
