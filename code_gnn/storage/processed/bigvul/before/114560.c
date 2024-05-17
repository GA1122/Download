void WebPluginProxy::UpdateGeometry(
    const gfx::Rect& window_rect,
    const gfx::Rect& clip_rect,
    const TransportDIB::Handle& windowless_buffer0,
    const TransportDIB::Handle& windowless_buffer1,
    int windowless_buffer_index,
    const TransportDIB::Handle& background_buffer,
    bool transparent) {
  gfx::Rect old = delegate_->GetRect();
  gfx::Rect old_clip_rect = delegate_->GetClipRect();
  transparent_ = transparent;

  if (TransportDIB::is_valid_handle(windowless_buffer0)) {
    SetWindowlessBuffers(windowless_buffer0,
                         windowless_buffer1,
                         background_buffer,
                         window_rect);
  }

  DCHECK(0 <= windowless_buffer_index && windowless_buffer_index <= 1);
  windowless_buffer_index_ = windowless_buffer_index;
#if defined(USE_X11)
  delegate_->SetWindowlessShmPixmap(windowless_shm_pixmap());
#endif

#if defined(OS_MACOSX)
  delegate_->UpdateGeometryAndContext(
      window_rect, clip_rect, windowless_context());
#else
  delegate_->UpdateGeometry(window_rect, clip_rect);
#endif

  if (delegate_->IsWindowless() && !clip_rect.IsEmpty() &&
      !damaged_rect_.IsEmpty()) {
    InvalidateRect(damaged_rect_);
  }
}
