void TestingAutomationProvider::UnblockAndLaunchBlockedPopup(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string error_message;
  TabContentsWrapper* tab_contents = GetTabContentsWrapperFromDict(
      browser, args, &error_message);
  if (!tab_contents) {
    reply.SendError(error_message);
    return;
  }
  int popup_index;
  if (!args->GetInteger("popup_index", &popup_index)) {
    reply.SendError("Need popup_index arg");
    return;
  }
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  BlockedContentTabHelper* blocked_content =
      tab_contents->blocked_content_tab_helper();
  if (popup_index >= (int)blocked_content->GetBlockedContentsCount()) {
    reply.SendError(StringPrintf("No popup at index %d", popup_index));
    return;
  }
  std::vector<TabContentsWrapper*> blocked_contents;
  blocked_content->GetBlockedContents(&blocked_contents);
  blocked_content->LaunchForContents(blocked_contents[popup_index]);
  reply.SendSuccess(NULL);
}
