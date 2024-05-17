void TestingAutomationProvider::CloseTabJSON(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  Browser* browser;
  WebContents* tab;
  std::string error;
  if (GetBrowserAndTabFromJSONArgs(args, &browser, &tab, &error)) {
    browser->CloseTabContents(tab);
    reply.SendSuccess(NULL);
    return;
  }

  RenderViewHost* view;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    reply.SendError(error);
    return;
  }
  view->ClosePage();
  reply.SendSuccess(NULL);
}
