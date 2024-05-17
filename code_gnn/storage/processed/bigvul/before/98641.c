void RenderWidget::OnHandleInputEvent(const IPC::Message& message) {
  void* iter = NULL;

  const char* data;
  int data_length;
  handling_input_event_ = true;
  if (!message.ReadData(&iter, &data, &data_length)) {
    handling_input_event_ = false;
    return;
  }

  const WebInputEvent* input_event =
      reinterpret_cast<const WebInputEvent*>(data);

  bool is_keyboard_shortcut = false;
  if (input_event->type == WebInputEvent::RawKeyDown)
    message.ReadBool(&iter, &is_keyboard_shortcut);

  bool processed = false;
  if (input_event->type != WebInputEvent::Char || !suppress_next_char_events_) {
    suppress_next_char_events_ = false;
    if (webwidget_)
      processed = webwidget_->handleInputEvent(*input_event);
  }

  if (!processed && is_keyboard_shortcut)
    suppress_next_char_events_ = true;

  IPC::Message* response = new ViewHostMsg_HandleInputEvent_ACK(routing_id_);
  response->WriteInt(input_event->type);
  response->WriteBool(processed);

  if (input_event->type == WebInputEvent::MouseMove &&
      paint_aggregator_.HasPendingUpdate()) {
    pending_input_event_ack_.reset(response);
  } else {
    Send(response);
  }

  handling_input_event_ = false;

  if (WebInputEvent::isKeyboardEventType(input_event->type))
    DidHandleKeyEvent();
}
