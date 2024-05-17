void Textfield::OnCursorBlinkTimerFired() {
  DCHECK(ShouldBlinkCursor());
  UpdateCursorViewPosition();
  cursor_view_.SetVisible(!cursor_view_.visible());
}
