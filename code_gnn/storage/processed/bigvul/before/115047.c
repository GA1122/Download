void TestingAutomationProvider::TriggerBrowserActionById(
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
  ExtensionAction* action = extension->browser_action();
  if (!action) {
    AutomationJSONReply(this, reply_message).SendError(
        "Extension doesn't have any browser action.");
    return;
  }
  EnsureTabSelected(browser, tab);

  BrowserActionTestUtil browser_actions(browser);
  int num_browser_actions = browser_actions.NumberOfBrowserActions();
  int action_index = -1;
#if defined(TOOLKIT_VIEWS)
  for (int i = 0; i < num_browser_actions; ++i) {
    if (extension->id() == browser_actions.GetExtensionId(i)) {
      action_index = i;
      break;
    }
  }
#else
  if (num_browser_actions != 1) {
    AutomationJSONReply(this, reply_message).SendError(StringPrintf(
        "Found %d browser actions. Only one browser action must be active.",
        num_browser_actions));
    return;
  }
  action_index = 0;
#endif
  if (action_index == -1) {
    AutomationJSONReply(this, reply_message).SendError(
        "Extension's browser action is not visible.");
    return;
  }
  browser_actions.Press(action_index);

  if (action->HasPopup(ExtensionTabUtil::GetTabId(tab))) {
    new ExtensionPopupObserver(
        this, reply_message, extension->id());
  } else {
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
  }
}
