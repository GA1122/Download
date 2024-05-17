void WebPluginDelegateProxy::CopyFromTransportToBacking(const gfx::Rect& rect) {
  if (!backing_store_canvas_.get()) {
    return;
  }

  gfx::Rect dest_rect = rect;
#if defined(OS_MACOSX)
  FlipRectVerticallyWithHeight(&dest_rect, plugin_rect_.height());
#endif
  BlitCanvasToCanvas(backing_store_canvas_.get(), dest_rect,
                     transport_store_canvas_.get(), rect.origin());
  backing_store_painted_ = backing_store_painted_.Union(rect);
}
