void RenderWidgetHostViewAura::InsertText(const base::string16& text) {
  DCHECK_NE(GetTextInputType(), ui::TEXT_INPUT_TYPE_NONE);

  if (text_input_manager_ && text_input_manager_->GetActiveWidget()) {
    if (text.length())
      text_input_manager_->GetActiveWidget()->ImeCommitText(
          text, std::vector<blink::WebCompositionUnderline>(),
          gfx::Range::InvalidRange(), 0);
    else if (has_composition_text_)
      text_input_manager_->GetActiveWidget()->ImeFinishComposingText(false);
  }
  has_composition_text_ = false;
}
