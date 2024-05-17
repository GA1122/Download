void TestingAutomationProvider::SetReleaseTrack(DictionaryValue* args,
                                                IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string track;
  if (!args->GetString("track", &track)) {
    reply.SendError("Invalid or missing args.");
    return;
  }

  DBusThreadManager::Get()->GetUpdateEngineClient()->SetReleaseTrack(track);
  reply.SendSuccess(NULL);
}
