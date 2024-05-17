void TestingAutomationProvider::GetTabIds(
    DictionaryValue* args, IPC::Message* reply_message) {
  ListValue* id_list = new ListValue();
  BrowserList::const_iterator iter = BrowserList::begin();
  for (; iter != BrowserList::end(); ++iter) {
    Browser* browser = *iter;
    for (int i = 0; i < browser->tab_count(); ++i) {
      int id = browser->GetTabContentsWrapperAt(i)->restore_tab_helper()->
          session_id().id();
      id_list->Append(Value::CreateIntegerValue(id));
    }
  }
  DictionaryValue dict;
  dict.Set("ids", id_list);
  AutomationJSONReply(this, reply_message).SendSuccess(&dict);
}
