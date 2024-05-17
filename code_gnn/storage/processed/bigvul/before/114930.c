void TestingAutomationProvider::GetNTPInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  new NTPInfoObserver(this, reply_message, &consumer_);
}
