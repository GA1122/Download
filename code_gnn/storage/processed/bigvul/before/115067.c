void TestingAutomationProvider::WebkitMouseDrag(DictionaryValue* args,
                                                IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  RenderViewHost* view;
  std::string error;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  WebKit::WebMouseEvent mouse_event;
  int start_x, start_y, end_x, end_y;
  if (!args->GetInteger("start_x", &start_x) ||
      !args->GetInteger("start_y", &start_y) ||
      !args->GetInteger("end_x", &end_x) ||
      !args->GetInteger("end_y", &end_y)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Invalid start/end positions");
    return;
  }

  mouse_event.type = WebKit::WebInputEvent::MouseMove;
  mouse_event.x = start_x;
  mouse_event.y = start_y;
  view->ForwardMouseEvent(mouse_event);

  mouse_event.type = WebKit::WebInputEvent::MouseDown;
  mouse_event.button = WebKit::WebMouseEvent::ButtonLeft;
  mouse_event.clickCount = 1;
  view->ForwardMouseEvent(mouse_event);

  mouse_event.type = WebKit::WebInputEvent::MouseMove;
  mouse_event.x = end_x;
  mouse_event.y = end_y;
  mouse_event.clickCount = 0;
  view->ForwardMouseEvent(mouse_event);

  mouse_event.type = WebKit::WebInputEvent::MouseUp;
  mouse_event.clickCount = 1;
  new InputEventAckNotificationObserver(this, reply_message, mouse_event.type,
                                        1);
  view->ForwardMouseEvent(mouse_event);
}
