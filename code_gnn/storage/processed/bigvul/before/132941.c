void RenderWidgetHostViewAura::FinishImeCompositionSession() {
  if (!has_composition_text_)
    return;
  if (host_) {
    host_->ImeConfirmComposition(base::string16(), gfx::Range::InvalidRange(),
                                 false);
  }
  ImeCancelComposition();
}
