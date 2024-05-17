bool SendGetTabIdsJSONRequest(
    AutomationMessageSender* sender,
    std::vector<int>* tab_ids,
    std::string* error_msg) {
  DictionaryValue dict;
  dict.SetString("command", "GetTabIds");
  DictionaryValue reply_dict;
  if (!SendAutomationJSONRequest(sender, dict, &reply_dict, error_msg))
    return false;
  ListValue* id_list;
  if (!reply_dict.GetList("ids", &id_list)) {
    LOG(ERROR) << "Returned 'ids' key is missing or invalid";
    return false;
  }
  std::vector<int> temp_ids;
  for (size_t i = 0; i < id_list->GetSize(); ++i) {
    int id;
    if (!id_list->GetInteger(i, &id)) {
      LOG(ERROR) << "Returned 'ids' key contains non-integer values";
      return false;
    }
    temp_ids.push_back(id);
  }
  *tab_ids = temp_ids;
  return true;
}
