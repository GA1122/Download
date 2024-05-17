void TestingAutomationProvider::SendWebkitKeyEvent(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  NativeWebKeyboardEvent event;
  std::string error;
  if (!BuildWebKeyEventFromArgs(args, &error, &event)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  RenderViewHost* view;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  new InputEventAckNotificationObserver(this, reply_message, event.type, 1);
  view->ForwardKeyboardEvent(event);
}
