void TestingAutomationProvider::GetConstrainedWindowCount(int handle,
                                                          int* count) {
  *count = -1;   
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* nav_controller = tab_tracker_->GetResource(handle);
    WebContents* web_contents = nav_controller->GetWebContents();
    if (web_contents) {
      TabContentsWrapper* wrapper =
          TabContentsWrapper::GetCurrentWrapperForContents(web_contents);
      if (wrapper) {
        *count = static_cast<int>(wrapper->constrained_window_tab_helper()->
                                  constrained_window_count());
      }
    }
  }
}
