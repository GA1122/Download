void WebPluginProxy::UpdateGeometry(
    const gfx::Rect& window_rect,
    const gfx::Rect& clip_rect,
    const TransportDIB::Handle& windowless_buffer,
    const TransportDIB::Handle& background_buffer
#if defined(OS_MACOSX)
    ,
    int ack_key
#endif
    ) {
  gfx::Rect old = delegate_->GetRect();
  gfx::Rect old_clip_rect = delegate_->GetClipRect();

  delegate_->UpdateGeometry(window_rect, clip_rect);
  if (TransportDIB::is_valid(windowless_buffer)) {
    SetWindowlessBuffer(windowless_buffer, background_buffer);
  }

  if (delegate_->IsWindowless() && !clip_rect.IsEmpty() &&
      old_clip_rect.IsEmpty() && !damaged_rect_.IsEmpty()) {
    InvalidateRect(damaged_rect_);
  }

#if defined(OS_MACOSX)
  if (ack_key != -1) {
    Send(new PluginHostMsg_UpdateGeometry_ACK(route_id_, ack_key));
  }
#endif
}
