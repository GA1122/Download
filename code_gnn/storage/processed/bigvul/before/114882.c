void TestingAutomationProvider::ExecuteJavascriptJSON(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  string16 frame_xpath, javascript;
  std::string error;
  RenderViewHost* render_view;
  if (!GetRenderViewFromJSONArgs(args, profile(), &render_view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  if (!args->GetString("frame_xpath", &frame_xpath)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'frame_xpath' missing or invalid");
    return;
  }
  if (!args->GetString("javascript", &javascript)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'javascript' missing or invalid");
    return;
  }

  new DomOperationMessageSender(this, reply_message, true);
  ExecuteJavascriptInRenderViewFrame(frame_xpath, javascript, reply_message,
                                     render_view);
}
