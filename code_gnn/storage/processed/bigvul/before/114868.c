void TestingAutomationProvider::DeleteCookie(const GURL& url,
                                             const std::string& cookie_name,
                                             int handle, bool* success) {
  WebContents* contents = tab_tracker_->ContainsHandle(handle) ?
      tab_tracker_->GetResource(handle)->GetWebContents() : NULL;
  automation_util::DeleteCookie(url, cookie_name, contents, success);
}
