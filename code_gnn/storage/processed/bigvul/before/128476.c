void ShellSurface::Configure() {
  DCHECK(widget_);

  if (scoped_configure_) {
    scoped_configure_->set_needs_configure();
    return;
  }

  gfx::Vector2d origin_offset = pending_origin_config_offset_;
  pending_origin_config_offset_ = gfx::Vector2d();

  int resize_component =
      resizer_ ? resizer_->details().window_component : HTCAPTION;

  if (configure_callback_.is_null()) {
    pending_origin_offset_ += origin_offset;
    pending_resize_component_ = resize_component;
    return;
  }

  uint32_t serial = configure_callback_.Run(
      widget_->GetWindowBoundsInScreen().size(),
      ash::wm::GetWindowState(widget_->GetNativeWindow())->GetStateType(),
      IsResizing(), widget_->IsActive());

  pending_configs_.push_back({serial, origin_offset, resize_component});
  LOG_IF(WARNING, pending_configs_.size() > 100)
      << "Number of pending configure acks for shell surface has reached: "
      << pending_configs_.size();
}
