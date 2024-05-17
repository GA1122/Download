void TestingAutomationProvider::ReloadJSON(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  WebContents* web_contents;
  std::string error;
  if (!GetTabFromJSONArgs(args, &web_contents, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  NavigationController& controller = web_contents->GetController();
  new NavigationNotificationObserver(&controller, this, reply_message,
                                     1, false, true);
  controller.Reload(false);
}
