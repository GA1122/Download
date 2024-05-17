void RenderWidgetHostViewAura::ConfirmCompositionText() {
  if (text_input_manager_ && text_input_manager_->GetActiveWidget() &&
      has_composition_text_) {
    text_input_manager_->GetActiveWidget()->ImeFinishComposingText(false);
  }
  has_composition_text_ = false;
}
