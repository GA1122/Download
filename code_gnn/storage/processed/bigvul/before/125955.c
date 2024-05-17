void PasswordStoreLoginsChangedObserver::IndicateDone() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (automation_) {
    if (result_key_.empty()) {
      AutomationJSONReply(automation_, reply_message_.release())
          .SendSuccess(NULL);
    } else {
      scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
      return_value->SetBoolean(result_key_, true);
      AutomationJSONReply(automation_, reply_message_.release())
          .SendSuccess(return_value.get());
    }
  }
  Release();
}
