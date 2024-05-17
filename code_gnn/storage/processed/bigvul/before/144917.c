bool RenderWidgetHostViewAura::GetTextFromRange(
    const gfx::Range& range,
    base::string16* text) const {
  if (!text_input_manager_ || !GetFocusedWidget())
    return false;

  const TextInputManager::TextSelection* selection =
      text_input_manager_->GetTextSelection(GetFocusedWidget()->GetView());
  if (!selection)
    return false;

  gfx::Range selection_text_range(
      selection->offset(), selection->offset() + selection->text().length());

  if (!selection_text_range.Contains(range)) {
    text->clear();
    return false;
  }
  if (selection_text_range.EqualsIgnoringDirection(range)) {
    *text = selection->text();
  } else {
    *text = selection->text().substr(range.GetMin() - selection->offset(),
                                     range.length());
  }
  return true;
}
