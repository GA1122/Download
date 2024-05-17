void ShellSurface::AcknowledgeConfigure(uint32_t serial) {
  TRACE_EVENT1("exo", "ShellSurface::AcknowledgeConfigure", "serial", serial);

  while (!pending_configs_.empty()) {
    auto config = pending_configs_.front();
    pending_configs_.pop_front();

    pending_origin_offset_ += config.origin_offset;

    pending_resize_component_ = config.resize_component;

    if (config.serial == serial)
      break;
  }

  if (widget_)
    UpdateWidgetBounds();
}
