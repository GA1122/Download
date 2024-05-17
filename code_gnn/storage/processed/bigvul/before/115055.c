void TestingAutomationProvider::WaitForAppModalDialogToBeShown(
    IPC::Message* reply_message) {
  if (AppModalDialogQueue::GetInstance()->HasActiveDialog()) {
    AutomationMsg_WaitForAppModalDialogToBeShown::WriteReplyParams(
        reply_message, true);
    Send(reply_message);
    return;
  }

  new AppModalDialogShownObserver(this, reply_message);
}
