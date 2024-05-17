void TestingAutomationProvider::WaitForAllViewsToStopLoading(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (AppModalDialogQueue::GetInstance()->HasActiveDialog()) {
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
    return;
  }

  new AllViewsStoppedLoadingObserver(
      this, reply_message, profile()->GetExtensionProcessManager());
}
