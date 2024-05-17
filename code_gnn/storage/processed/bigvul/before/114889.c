void TestingAutomationProvider::GetAllNotifications(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  new GetAllNotificationsObserver(this, reply_message);
}
