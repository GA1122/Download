void TestingAutomationProvider::GetBlockedPopupCount(int handle, int* count) {
  *count = -1;   
  if (tab_tracker_->ContainsHandle(handle)) {
      NavigationController* nav_controller = tab_tracker_->GetResource(handle);
      TabContentsWrapper* tab_contents =
          TabContentsWrapper::GetCurrentWrapperForContents(
              nav_controller->GetWebContents());
      if (tab_contents) {
        BlockedContentTabHelper* blocked_content =
            tab_contents->blocked_content_tab_helper();
        *count = static_cast<int>(blocked_content->GetBlockedContentsCount());
      }
  }
}
