void WebPluginProxy::SetWindowlessBuffers(
    const TransportDIB::Handle& windowless_buffer0,
    const TransportDIB::Handle& windowless_buffer1,
    const TransportDIB::Handle& background_buffer,
    const gfx::Rect& window_rect) {
  CreateDIBAndCGContextFromHandle(windowless_buffer0,
                                  window_rect,
                                  &windowless_dibs_[0],
                                  &windowless_contexts_[0]);
  CreateDIBAndCGContextFromHandle(windowless_buffer1,
                                  window_rect,
                                  &windowless_dibs_[1],
                                  &windowless_contexts_[1]);
  CreateDIBAndCGContextFromHandle(background_buffer,
                                  window_rect,
                                  &background_dib_,
                                  &background_context_);
}
