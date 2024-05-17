void TestingAutomationProvider::IsTabIdValid(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  int id;
  if (!args->GetInteger("id", &id)) {
    reply.SendError("'id' missing or invalid");
    return;
  }
  bool is_valid = false;
  BrowserList::const_iterator iter = BrowserList::begin();
  for (; iter != BrowserList::end(); ++iter) {
    Browser* browser = *iter;
    for (int i = 0; i < browser->tab_count(); ++i) {
      TabContentsWrapper* tab = browser->GetTabContentsWrapperAt(i);
      if (tab->restore_tab_helper()->session_id().id() == id) {
        is_valid = true;
        break;
      }
    }
  }
  DictionaryValue dict;
  dict.SetBoolean("is_valid", is_valid);
  reply.SendSuccess(&dict);
}
