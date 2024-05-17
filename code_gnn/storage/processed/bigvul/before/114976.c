void TestingAutomationProvider::IsPageActionVisible(
    base::DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);

  WebContents* tab;
  std::string error;
  if (!GetTabFromJSONArgs(args, &tab, &error)) {
    reply.SendError(error);
    return;
  }
  const Extension* extension;
  if (!GetEnabledExtensionFromJSONArgs(
          args, "extension_id", profile(), &extension, &error)) {
    reply.SendError(error);
    return;
  }
  ExtensionAction* page_action = extension->page_action();
  if (!page_action) {
    reply.SendError("Extension doesn't have any page action");
    return;
  }
  Browser* browser = automation_util::GetBrowserForTab(tab);
  if (!browser) {
    reply.SendError("Tab does not belong to an open browser");
    return;
  }
  EnsureTabSelected(browser, tab);

  bool is_visible = false;
  LocationBarTesting* loc_bar =
      browser->window()->GetLocationBar()->GetLocationBarForTesting();
  size_t page_action_visible_count =
      static_cast<size_t>(loc_bar->PageActionVisibleCount());
  for (size_t i = 0; i < page_action_visible_count; ++i) {
    if (loc_bar->GetVisiblePageAction(i) == page_action) {
      is_visible = true;
      break;
    }
  }
  DictionaryValue dict;
  dict.SetBoolean("is_visible", is_visible);
  reply.SendSuccess(&dict);
}
