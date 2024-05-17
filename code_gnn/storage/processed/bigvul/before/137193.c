int Textfield::GetDragSelectionDelay() const {
  switch (ui::ScopedAnimationDurationScaleMode::duration_scale_mode()) {
    case ui::ScopedAnimationDurationScaleMode::NORMAL_DURATION:
      return 100;
    case ui::ScopedAnimationDurationScaleMode::FAST_DURATION:
      return 25;
    case ui::ScopedAnimationDurationScaleMode::SLOW_DURATION:
      return 400;
    case ui::ScopedAnimationDurationScaleMode::NON_ZERO_DURATION:
      return 1;
    case ui::ScopedAnimationDurationScaleMode::ZERO_DURATION:
      return 0;
  }
  return 100;
}
