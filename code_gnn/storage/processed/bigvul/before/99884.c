void WebPluginDelegateProxy::OnInvalidateRect(const gfx::Rect& rect) {
  if (!plugin_)
    return;

  invalidate_pending_ = true;
  CopyFromTransportToBacking(rect);
  plugin_->InvalidateRect(rect);
 }
