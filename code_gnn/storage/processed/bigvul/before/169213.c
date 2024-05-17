InputEventAckWaiter::InputEventAckPredicate EventAckHasType(
    blink::WebInputEvent::Type type) {
  return base::BindRepeating(
      [](blink::WebInputEvent::Type expected_type, InputEventAckSource source,
         InputEventAckState state, const blink::WebInputEvent& event) {
        return event.GetType() == expected_type;
      },
      type);
}
