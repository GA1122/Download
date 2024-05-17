void NavigationControllerRestoredObserver::SendDone() {
  if (automation_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(NULL);
  }
  delete this;
}
