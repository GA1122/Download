bool SelectionController::HandlePasteGlobalSelection(
    const WebMouseEvent& mouse_event) {
  if (mouse_event.GetType() != WebInputEvent::kMouseUp)
    return false;

  if (!frame_->GetPage())
    return false;
  Frame* focus_frame =
      frame_->GetPage()->GetFocusController().FocusedOrMainFrame();
  if (frame_ == focus_frame &&
      frame_->GetEditor().Behavior().SupportsGlobalSelection())
    return frame_->GetEditor().CreateCommand("PasteGlobalSelection").Execute();

  return false;
}
