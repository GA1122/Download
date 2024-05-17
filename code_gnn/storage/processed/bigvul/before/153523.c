float GM2TabStyle::GetThrobValue() const {
  const bool is_selected = tab_->IsSelected();
  double val = is_selected ? kSelectedTabOpacity : 0;

  if (IsHoverActive()) {
    constexpr float kSelectedTabThrobScale = 0.95f - kSelectedTabOpacity;
    const float opacity = GetHoverOpacity();
    const float offset =
        is_selected ? (kSelectedTabThrobScale * opacity) : opacity;
    val += GetHoverAnimationValue() * offset;
  }

  return val;
}
