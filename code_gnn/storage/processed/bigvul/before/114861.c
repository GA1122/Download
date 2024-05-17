void TestingAutomationProvider::ClickInfoBarAccept(
    int handle,
    size_t info_bar_index,
    bool wait_for_navigation,
    IPC::Message* reply_message) {
  bool success = false;
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* nav_controller = tab_tracker_->GetResource(handle);
    if (nav_controller) {
      InfoBarTabHelper* infobar_helper =
          TabContentsWrapper::GetCurrentWrapperForContents(
              nav_controller->GetWebContents())->infobar_tab_helper();
      if (info_bar_index < infobar_helper->infobar_count()) {
        if (wait_for_navigation) {
          new NavigationNotificationObserver(nav_controller, this,
                                             reply_message, 1, false, false);
        }
        InfoBarDelegate* delegate =
            infobar_helper->GetInfoBarDelegateAt(info_bar_index);
        if (delegate->AsConfirmInfoBarDelegate())
          delegate->AsConfirmInfoBarDelegate()->Accept();
        success = true;
      }
    }
  }

  if (!wait_for_navigation || !success)
    AutomationMsg_ClickInfoBarAccept::WriteReplyParams(
        reply_message, AUTOMATION_MSG_NAVIGATION_ERROR);
}
