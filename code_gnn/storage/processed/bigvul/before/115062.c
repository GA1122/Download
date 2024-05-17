void TestingAutomationProvider::WaitForTabCountToBecome(
    int browser_handle,
    int target_tab_count,
    IPC::Message* reply_message) {
  if (!browser_tracker_->ContainsHandle(browser_handle)) {
    AutomationMsg_WaitForTabCountToBecome::WriteReplyParams(reply_message,
                                                            false);
    Send(reply_message);
    return;
  }

  Browser* browser = browser_tracker_->GetResource(browser_handle);

  new TabCountChangeObserver(this, browser, reply_message, target_tab_count);
}
