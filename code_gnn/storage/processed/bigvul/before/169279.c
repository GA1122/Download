void InputMsgWatcher::OnInputEventAck(InputEventAckSource ack_source,
                                      InputEventAckState ack_state,
                                      const blink::WebInputEvent& event) {
  if (event.GetType() == wait_for_type_) {
    ack_result_ = ack_state;
    ack_source_ = ack_source;
    if (!quit_.is_null())
      quit_.Run();
  }
}
