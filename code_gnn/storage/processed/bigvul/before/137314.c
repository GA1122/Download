void Textfield::UpdateCursorVisibility() {
  cursor_view_.SetVisible(ShouldShowCursor());
  if (ShouldBlinkCursor())
    StartBlinkingCursor();
  else
    StopBlinkingCursor();
}
