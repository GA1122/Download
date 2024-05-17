void WebPluginProxy::SetWindowlessBuffer(
    const TransportDIB::Handle& windowless_buffer,
    const TransportDIB::Handle& background_buffer) {
  windowless_canvas_.reset(new skia::PlatformCanvas);
  if (!windowless_canvas_->initialize(
          delegate_->GetRect().width(),
          delegate_->GetRect().height(),
          true,
          win_util::GetSectionFromProcess(windowless_buffer,
              channel_->renderer_handle(), false))) {
    windowless_canvas_.reset();
    background_canvas_.reset();
    return;
  }

  if (background_buffer) {
    background_canvas_.reset(new skia::PlatformCanvas);
    if (!background_canvas_->initialize(
            delegate_->GetRect().width(),
            delegate_->GetRect().height(),
            true,
            win_util::GetSectionFromProcess(background_buffer,
                channel_->renderer_handle(), false))) {
      windowless_canvas_.reset();
      background_canvas_.reset();
      return;
    }
  }
}
