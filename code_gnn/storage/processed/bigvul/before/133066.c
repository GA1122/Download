void HWNDMessageHandler::ClientAreaSizeChanged() {
  gfx::Size s = GetClientAreaBounds().size();
  delegate_->HandleClientSizeChanged(s);
  if (use_layered_buffer_)
    layered_window_contents_.reset(new gfx::Canvas(s, 1.0f, false));
}
