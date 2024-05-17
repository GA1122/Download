bool GM2TabStyle::IsHoverActive() const {
  if (!hover_controller_)
    return false;
  return hover_controller_->ShouldDraw();
}
