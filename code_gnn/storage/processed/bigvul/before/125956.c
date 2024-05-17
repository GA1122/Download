void PasswordStoreLoginsChangedObserver::IndicateError(
    const std::string& error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (automation_)
    AutomationJSONReply(automation_, reply_message_.release()).SendError(error);
  Release();
}
