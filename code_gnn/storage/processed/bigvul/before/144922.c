bool RenderWidgetHostViewAura::GetTextRange(gfx::Range* range) const {
  if (!text_input_manager_ || !GetFocusedWidget())
    return false;

  const TextInputManager::TextSelection* selection =
      text_input_manager_->GetTextSelection(GetFocusedWidget()->GetView());
  if (!selection)
    return false;

  range->set_start(selection->offset());
  range->set_end(selection->offset() + selection->text().length());
  return true;
}
