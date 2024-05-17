void TestingAutomationProvider::WebkitMouseButtonUp(
    DictionaryValue* args, IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  RenderViewHost* view;
  std::string error;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  WebKit::WebMouseEvent mouse_event;
  if (!args->GetInteger("x", &mouse_event.x) ||
      !args->GetInteger("y", &mouse_event.y)) {
    AutomationJSONReply(this, reply_message)
        .SendError("(X,Y) coordinates missing or invalid");
    return;
  }

  mouse_event.type = WebKit::WebInputEvent::MouseUp;
  mouse_event.button = WebKit::WebMouseEvent::ButtonLeft;
  mouse_event.clickCount = 1;
  new InputEventAckNotificationObserver(this, reply_message, mouse_event.type,
                                        1);
  view->ForwardMouseEvent(mouse_event);
}
