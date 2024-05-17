void WebPluginDelegateProxy::UpdateGeometry(const gfx::Rect& window_rect,
                                            const gfx::Rect& clip_rect) {
  if (window_rect.width() < 0  || window_rect.width() > kMaxPluginSideLength ||
      window_rect.height() < 0 || window_rect.height() > kMaxPluginSideLength ||
      static_cast<uint32>(window_rect.width()) *
          static_cast<uint32>(window_rect.height()) > kMaxPluginSize) {
    return;
  }

  plugin_rect_ = window_rect;
  clip_rect_ = clip_rect;

  bool bitmaps_changed = false;

  if (uses_shared_bitmaps_) {
    if (!front_buffer_canvas() ||
        (window_rect.width() != front_buffer_canvas()->getDevice()->width() ||
         window_rect.height() != front_buffer_canvas()->getDevice()->height()))
    {
      bitmaps_changed = true;

      bool needs_background_store = transparent_;
#if defined(OS_MACOSX)
      needs_background_store = false;
#endif

      ResetWindowlessBitmaps();
      if (!window_rect.IsEmpty()) {
        if (!CreateSharedBitmap(&transport_stores_[0].dib,
                                &transport_stores_[0].canvas) ||
            !CreateSharedBitmap(&transport_stores_[1].dib,
                                &transport_stores_[1].canvas) ||
            (needs_background_store &&
             !CreateSharedBitmap(&background_store_.dib,
                                 &background_store_.canvas))) {
          DCHECK(false);
          ResetWindowlessBitmaps();
          return;
        }
      }
    }
  }

  SendUpdateGeometry(bitmaps_changed);
}
