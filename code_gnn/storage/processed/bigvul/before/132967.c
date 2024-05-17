void RenderWidgetHostViewAura::InsertText(const base::string16& text) {
  DCHECK(text_input_type_ != ui::TEXT_INPUT_TYPE_NONE);
  if (host_)
    host_->ImeConfirmComposition(text, gfx::Range::InvalidRange(), false);
  has_composition_text_ = false;
}
