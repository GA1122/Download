bool WebPluginDelegateProxy::HandleInputEvent(
    const WebInputEvent& event,
    WebCursorInfo* cursor_info) {
  bool handled;
  WebCursor cursor;
  IPC::SyncMessage* message = new PluginMsg_HandleInputEvent(
      instance_id_, &event, &handled, &cursor);
  message->set_pump_messages_event(modal_loop_pump_messages_event_.get());
  Send(message);
  cursor.GetCursorInfo(cursor_info);
  return handled;
}
