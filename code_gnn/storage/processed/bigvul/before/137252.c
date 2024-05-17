void Textfield::OnDragExited() {
  drop_cursor_visible_ = false;
  if (ShouldBlinkCursor())
    StartBlinkingCursor();
  SchedulePaint();
}
