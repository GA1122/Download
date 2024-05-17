void InputEventAckWaiter::OnInputEventAck(InputEventAckSource source,
                                          InputEventAckState state,
                                          const blink::WebInputEvent& event) {
  if (predicate_.Run(source, state, event)) {
    event_received_ = true;
    if (quit_)
      quit_.Run();
  }
}
