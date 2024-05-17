void TestingAutomationProvider::TriggerPageActionById(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  std::string error;
  Browser* browser;
  WebContents* tab;
  if (!GetBrowserAndTabFromJSONArgs(args, &browser, &tab, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  const Extension* extension;
  if (!GetEnabledExtensionFromJSONArgs(
          args, "id", profile(), &extension, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  ExtensionAction* page_action = extension->page_action();
  if (!page_action) {
    AutomationJSONReply(this, reply_message).SendError(
        "Extension doesn't have any page action.");
    return;
  }
  EnsureTabSelected(browser, tab);

  bool pressed = false;
  LocationBarTesting* loc_bar =
      browser->window()->GetLocationBar()->GetLocationBarForTesting();
  size_t page_action_visible_count =
      static_cast<size_t>(loc_bar->PageActionVisibleCount());
  for (size_t i = 0; i < page_action_visible_count; ++i) {
    if (loc_bar->GetVisiblePageAction(i) == page_action) {
      loc_bar->TestPageActionPressed(i);
      pressed = true;
      break;
    }
  }
  if (!pressed) {
    AutomationJSONReply(this, reply_message).SendError(
        "Extension's page action is not visible.");
    return;
  }

  if (page_action->HasPopup(ExtensionTabUtil::GetTabId(tab))) {
    new ExtensionPopupObserver(
        this, reply_message, extension->id());
  } else {
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
  }
}
