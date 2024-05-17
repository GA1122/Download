void WebPluginDelegateProxy::UpdateFrontBuffer(
    const gfx::Rect& rect,
    bool allow_buffer_flipping) {
  if (!front_buffer_canvas()) {
    return;
  }

#if defined(OS_WIN)
  if (UseSynchronousGeometryUpdates()) {
    allow_buffer_flipping = false;
  }
#endif

  front_buffer_diff_ = front_buffer_diff_.Subtract(rect);
  if (allow_buffer_flipping && front_buffer_diff_.IsEmpty()) {
    front_buffer_index_ = back_buffer_index();
    SendUpdateGeometry(false);
    front_buffer_diff_ = rect;
  } else {
    CopyFromBackBufferToFrontBuffer(rect);
  }
  transport_store_painted_ = transport_store_painted_.Union(rect);
}
