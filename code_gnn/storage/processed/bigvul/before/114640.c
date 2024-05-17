void WebPluginDelegateProxy::OnInvalidateRect(const gfx::Rect& rect,
                                              bool allow_buffer_flipping) {
  if (!plugin_)
    return;

  const gfx::Rect clipped_rect(rect.Intersect(gfx::Rect(plugin_rect_.size())));

  invalidate_pending_ = true;
  UpdateFrontBuffer(clipped_rect, allow_buffer_flipping);
  plugin_->InvalidateRect(clipped_rect);
}
