void WebPluginProxy::SetWindowlessBuffers(
    const TransportDIB::Handle& windowless_buffer0,
    const TransportDIB::Handle& windowless_buffer1,
    const TransportDIB::Handle& background_buffer,
    const gfx::Rect& window_rect) {
  CreateDIBAndCanvasFromHandle(windowless_buffer0,
                               window_rect,
                               &windowless_dibs_[0],
                               &windowless_canvases_[0]);
  CreateDIBAndCanvasFromHandle(windowless_buffer1,
                               window_rect,
                               &windowless_dibs_[1],
                               &windowless_canvases_[1]);
  CreateDIBAndCanvasFromHandle(background_buffer,
                               window_rect,
                               &background_dib_,
                               &background_canvas_);

  if (delegate_->IsWindowless() && use_shm_pixmap_) {
    CreateShmPixmapFromDIB(windowless_dibs_[0].get(),
                           window_rect,
                           &windowless_shm_pixmaps_[0]);
    CreateShmPixmapFromDIB(windowless_dibs_[1].get(),
                           window_rect,
                           &windowless_shm_pixmaps_[1]);
  }
}
