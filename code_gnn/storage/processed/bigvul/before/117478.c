void TestingAutomationProvider::GetUpdateInfo(DictionaryValue* args,
                                              IPC::Message* reply_message) {
  AutomationJSONReply* reply = new AutomationJSONReply(this, reply_message);
  DBusThreadManager::Get()->GetUpdateEngineClient()
      ->GetReleaseTrack(base::Bind(GetReleaseTrackCallback, reply));
}
