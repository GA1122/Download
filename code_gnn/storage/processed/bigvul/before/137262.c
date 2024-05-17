bool Textfield::OnMousePressed(const ui::MouseEvent& event) {
  const bool had_focus = HasFocus();
  bool handled = controller_ && controller_->HandleMouseEvent(this, event);
  if (!handled &&
      (event.IsOnlyLeftMouseButton() || event.IsOnlyRightMouseButton())) {
    if (!had_focus)
      RequestFocus();
    ShowImeIfNeeded();
  }

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  if (!handled && !had_focus && event.IsOnlyMiddleMouseButton())
    RequestFocus();
#endif

  return selection_controller_.OnMousePressed(
      event, handled,
      had_focus ? SelectionController::FOCUSED
                : SelectionController::UNFOCUSED);
}
