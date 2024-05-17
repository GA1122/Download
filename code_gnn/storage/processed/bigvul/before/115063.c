void TestingAutomationProvider::WaitForTabToBeRestored(
    int tab_handle,
    IPC::Message* reply_message) {
  if (tab_tracker_->ContainsHandle(tab_handle)) {
    NavigationController* tab = tab_tracker_->GetResource(tab_handle);
    restore_tracker_.reset(
        new NavigationControllerRestoredObserver(this, tab, reply_message));
  } else {
    AutomationMsg_WaitForTabToBeRestored::WriteReplyParams(
        reply_message, false);
    Send(reply_message);
  }
}
