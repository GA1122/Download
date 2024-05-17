void TestingAutomationProvider::ProcessWebMouseEvent(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  RenderViewHost* view;
  std::string error;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  int type;
  int button;
  int modifiers;
  WebKit::WebMouseEvent event;
  if (!args->GetInteger("type", &type) ||
      !args->GetInteger("button", &button) ||
      !args->GetInteger("x", &event.x) ||
      !args->GetInteger("y", &event.y) ||
      !args->GetInteger("click_count", &event.clickCount) ||
      !args->GetInteger("modifiers", &modifiers)) {
    AutomationJSONReply(this, reply_message)
        .SendError("WebMouseEvent has missing or invalid parameters");
    return;
  }
  if (type == automation::kMouseDown) {
    event.type = WebKit::WebInputEvent::MouseDown;
  } else if (type == automation::kMouseUp) {
    event.type = WebKit::WebInputEvent::MouseUp;
  } else if (type == automation::kMouseMove) {
    event.type = WebKit::WebInputEvent::MouseMove;
  } else if (type == automation::kMouseEnter) {
    event.type = WebKit::WebInputEvent::MouseEnter;
  } else if (type == automation::kMouseLeave) {
    event.type = WebKit::WebInputEvent::MouseLeave;
  } else if (type == automation::kContextMenu) {
    event.type = WebKit::WebInputEvent::ContextMenu;
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError("'type' refers to an unrecognized mouse event type");
    return;
  }
  if (button == automation::kLeftButton) {
    event.button = WebKit::WebMouseEvent::ButtonLeft;
  } else if (button == automation::kMiddleButton) {
    event.button = WebKit::WebMouseEvent::ButtonMiddle;
  } else if (button == automation::kRightButton) {
    event.button = WebKit::WebMouseEvent::ButtonRight;
  } else if (button == automation::kNoButton) {
    event.button = WebKit::WebMouseEvent::ButtonNone;
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError("'button' refers to an unrecognized button");
    return;
  }
  event.modifiers = 0;
  if (modifiers & automation::kShiftKeyMask)
    event.modifiers |= WebKit::WebInputEvent::ShiftKey;
  if (modifiers & automation::kControlKeyMask)
    event.modifiers |= WebKit::WebInputEvent::ControlKey;
  if (modifiers & automation::kAltKeyMask)
    event.modifiers |= WebKit::WebInputEvent::AltKey;
  if (modifiers & automation::kMetaKeyMask)
    event.modifiers |= WebKit::WebInputEvent::MetaKey;

  view->ForwardMouseEvent(event);
  new InputEventAckNotificationObserver(this, reply_message, event.type,
                                        1);
}
