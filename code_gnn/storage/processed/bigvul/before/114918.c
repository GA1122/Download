void TestingAutomationProvider::GetIndicesFromTab(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  int id_or_handle = 0;
  bool has_id = args->HasKey("tab_id");
  bool has_handle = args->HasKey("tab_handle");
  if (has_id && has_handle) {
    reply.SendError(
        "Both 'tab_id' and 'tab_handle' were specified. Only one is allowed");
    return;
  } else if (!has_id && !has_handle) {
    reply.SendError("Either 'tab_id' or 'tab_handle' must be specified");
    return;
  }
  if (has_id && !args->GetInteger("tab_id", &id_or_handle)) {
    reply.SendError("'tab_id' is invalid");
    return;
  }
  if (has_handle && (!args->GetInteger("tab_handle", &id_or_handle) ||
                     !tab_tracker_->ContainsHandle(id_or_handle))) {
    reply.SendError("'tab_handle' is invalid");
    return;
  }
  int id = id_or_handle;
  if (has_handle) {
    TabContentsWrapper* tab = TabContentsWrapper::GetCurrentWrapperForContents(
        tab_tracker_->GetResource(id_or_handle)->GetWebContents());
    id = tab->restore_tab_helper()->session_id().id();
  }
  BrowserList::const_iterator iter = BrowserList::begin();
  int browser_index = 0;
  for (; iter != BrowserList::end(); ++iter, ++browser_index) {
    Browser* browser = *iter;
    for (int tab_index = 0; tab_index < browser->tab_count(); ++tab_index) {
      TabContentsWrapper* tab = browser->GetTabContentsWrapperAt(tab_index);
      if (tab->restore_tab_helper()->session_id().id() == id) {
        DictionaryValue dict;
        dict.SetInteger("windex", browser_index);
        dict.SetInteger("tab_index", tab_index);
        reply.SendSuccess(&dict);
        return;
      }
    }
  }
  reply.SendError("Could not find tab among current browser windows");
}
