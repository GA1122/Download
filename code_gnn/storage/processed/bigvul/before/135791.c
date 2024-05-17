bool IsExtendingSelection(const MouseEventWithHitTestResults& event) {
  bool is_mouse_down_on_link_or_image =
      event.IsOverLink() || event.GetHitTestResult().GetImage();
  return (event.Event().GetModifiers() & WebInputEvent::Modifiers::kShiftKey) !=
             0 &&
         !is_mouse_down_on_link_or_image;
}
