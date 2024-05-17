void TestingAutomationProvider::GetInfoBarCount(int handle, size_t* count) {
  *count = static_cast<size_t>(-1);   
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* nav_controller = tab_tracker_->GetResource(handle);
    if (nav_controller) {
      TabContentsWrapper* wrapper =
          TabContentsWrapper::GetCurrentWrapperForContents(
              nav_controller->GetWebContents());
      *count = wrapper->infobar_tab_helper()->infobar_count();
    }
  }
}
