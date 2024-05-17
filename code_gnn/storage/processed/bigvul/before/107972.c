void TranslateInfoBarBase::PaintBackground(gfx::Canvas* canvas) {
  if (background_color_animation_ == NULL ||
      !background_color_animation_->is_animating()) {
    GetBackground().Paint(canvas, this);
    return;
  }

  FadeBackground(canvas, 1.0 - background_color_animation_->GetCurrentValue(),
                 normal_background_);
  FadeBackground(canvas, background_color_animation_->GetCurrentValue(),
                 error_background_);
}
