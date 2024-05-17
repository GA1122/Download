void GM2TabStyle::SetHoverLocation(const gfx::Point& location) {
  if (hover_controller_)
    hover_controller_->SetLocation(location);
}
