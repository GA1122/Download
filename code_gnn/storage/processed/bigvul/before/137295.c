void Textfield::SetText(const base::string16& new_text) {
  model_->SetText(new_text);
  OnCaretBoundsChanged();
  UpdateCursorViewPosition();
  UpdateCursorVisibility();
  SchedulePaint();
  NotifyAccessibilityEvent(ax::mojom::Event::kValueChanged, true);
}
