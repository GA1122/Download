void TestingAutomationProvider::DoesAutomationObjectExist(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  AutomationId id;
  std::string error_msg;
  if (!GetAutomationIdFromJSONArgs(args, "auto_id", &id, &error_msg)) {
    reply.SendError(error_msg);
    return;
  }
  DictionaryValue dict;
  dict.SetBoolean(
      "does_exist",
      automation_util::DoesObjectWithIdExist(id, profile()));
  reply.SendSuccess(&dict);
}
