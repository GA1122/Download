bool IsLinkSelection(const MouseEventWithHitTestResults& event) {
  return (event.Event().GetModifiers() & WebInputEvent::Modifiers::kAltKey) !=
             0 &&
         event.IsOverLink();
}
