void TestingAutomationProvider::ActivateTabJSON(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  AutomationJSONReply reply(this, reply_message);
  Browser* browser;
  WebContents* web_contents;
  std::string error;
  if (!GetBrowserAndTabFromJSONArgs(args, &browser, &web_contents, &error)) {
    reply.SendError(error);
    return;
  }
  browser->ActivateTabAt(
      browser->GetIndexOfController(&web_contents->GetController()), true);
  reply.SendSuccess(NULL);
}
