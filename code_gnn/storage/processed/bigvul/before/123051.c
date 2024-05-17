void RenderWidgetHostImpl::SendInputEvent(const WebInputEvent& input_event,
                                          int event_size,
                                          bool is_keyboard_shortcut) {
  IPC::Message* message = new ViewMsg_HandleInputEvent(routing_id_);
  message->WriteData(
      reinterpret_cast<const char*>(&input_event), event_size);
  if (input_event.type == WebInputEvent::RawKeyDown)
    message->WriteBool(is_keyboard_shortcut);
  input_event_start_time_ = TimeTicks::Now();
  Send(message);
  increment_in_flight_event_count();
}
