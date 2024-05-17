double GM2TabStyle::GetHoverAnimationValue() const {
  if (!hover_controller_)
    return 0.0;
  return hover_controller_->GetAnimationValue();
}
