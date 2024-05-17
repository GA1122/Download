void TestingAutomationProvider::WaitForInfoBarCount(
    int tab_handle,
    size_t target_count,
    IPC::Message* reply_message) {
  if (!tab_tracker_->ContainsHandle(tab_handle)) {
    AutomationMsg_WaitForInfoBarCount::WriteReplyParams(reply_message_, false);
    Send(reply_message_);
    return;
  }

  NavigationController* controller = tab_tracker_->GetResource(tab_handle);
  if (!controller) {
    AutomationMsg_WaitForInfoBarCount::WriteReplyParams(reply_message_, false);
    Send(reply_message_);
    return;
  }

  new InfoBarCountObserver(this, reply_message,
      TabContentsWrapper::GetCurrentWrapperForContents(
          controller->GetWebContents()), target_count);
}
