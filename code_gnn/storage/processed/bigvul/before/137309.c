void Textfield::UpdateAfterChange(bool text_changed, bool cursor_changed) {
  if (text_changed) {
    if (controller_)
      controller_->ContentsChanged(this, text());
    NotifyAccessibilityEvent(ax::mojom::Event::kValueChanged, true);
  }
  if (cursor_changed) {
    UpdateCursorViewPosition();
    UpdateCursorVisibility();
  }
  if (text_changed || cursor_changed) {
    OnCaretBoundsChanged();
    SchedulePaint();
  }
}
