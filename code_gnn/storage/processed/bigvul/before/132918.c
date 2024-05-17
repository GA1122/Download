void RenderWidgetHostViewAura::ConfirmCompositionText() {
  if (host_ && has_composition_text_) {
    host_->ImeConfirmComposition(base::string16(), gfx::Range::InvalidRange(),
                                 false);
  }
  has_composition_text_ = false;
}
