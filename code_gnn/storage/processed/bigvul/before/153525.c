float GM2TabStyle::GetZValue() const {
  float sort_value = GetHoverAnimationValue();
  if (tab_->IsSelected())
    sort_value += 4.f;
  if (tab_->mouse_hovered())
    sort_value += 2.f;
  return sort_value;
}
