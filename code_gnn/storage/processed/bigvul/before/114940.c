void TestingAutomationProvider::GetPolicyDefinitionList(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);

#if !defined(ENABLE_CONFIGURATION_POLICY)
  reply.SendError("Configuration Policy disabled");
#else
  DictionaryValue response;

  const policy::PolicyDefinitionList* list =
      policy::GetChromePolicyDefinitionList();
  std::map<Value::Type, std::string> types;
  types[Value::TYPE_BOOLEAN] = "bool";
  types[Value::TYPE_DICTIONARY] = "dict";
  types[Value::TYPE_INTEGER] = "int";
  types[Value::TYPE_LIST] = "list";
  types[Value::TYPE_STRING] = "str";

  const policy::PolicyDefinitionList::Entry* entry;
  for (entry = list->begin; entry != list->end; ++entry) {
    if (types.find(entry->value_type) == types.end()) {
      std::string error("Unrecognized policy type for policy ");
      reply.SendError(error + entry->name);
      return;
    }
    response.SetString(entry->name, types[entry->value_type]);
  }

  reply.SendSuccess(&response);
#endif
}
