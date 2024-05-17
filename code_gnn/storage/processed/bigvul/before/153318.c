void Tab::AnimationProgressed(const gfx::Animation* animation) {
  DCHECK_EQ(animation, &title_animation_);
  title_->SetBoundsRect(gfx::Tween::RectValueBetween(
      gfx::Tween::CalculateValue(gfx::Tween::FAST_OUT_SLOW_IN,
                                 animation->GetCurrentValue()),
      start_title_bounds_, target_title_bounds_));
}
