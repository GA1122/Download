void WebPluginDelegateProxy::UpdateGeometry(const gfx::Rect& window_rect,
                                            const gfx::Rect& clip_rect) {
  if (window_rect.width() < 0  || window_rect.width() > (1<<15) ||
      window_rect.height() < 0 || window_rect.height() > (1<<15) ||
      window_rect.width() * window_rect.height() > (8<<20)) {
    return;
  }

  plugin_rect_ = window_rect;

  bool bitmaps_changed = false;

  PluginMsg_UpdateGeometry_Param param;
#if defined(OS_MACOSX)
  param.ack_key = -1;
#endif

  if (uses_shared_bitmaps_) {
    if (!backing_store_canvas_.get() ||
        (window_rect.width() != backing_store_canvas_->getDevice()->width() ||
         window_rect.height() != backing_store_canvas_->getDevice()->height()))
    {
      bitmaps_changed = true;

      bool needs_background_store = transparent_;
#if defined(OS_MACOSX)
      needs_background_store = false;
      if (transport_store_.get()) {
        param.ack_key = transport_store_->handle().fd;
      }
#endif

      ResetWindowlessBitmaps();
      if (!window_rect.IsEmpty()) {
        if (!CreateSharedBitmap(&transport_store_, &transport_store_canvas_) ||
#if defined(OS_WIN)
            !CreateSharedBitmap(&backing_store_, &backing_store_canvas_) ||
#else
            !CreateLocalBitmap(&backing_store_, &backing_store_canvas_) ||
#endif
            (needs_background_store &&
             !CreateSharedBitmap(&background_store_,
                                 &background_store_canvas_))) {
          DCHECK(false);
          ResetWindowlessBitmaps();
          return;
        }
      }
    }
  }

  param.window_rect = window_rect;
  param.clip_rect = clip_rect;
  param.windowless_buffer = TransportDIB::DefaultHandleValue();
  param.background_buffer = TransportDIB::DefaultHandleValue();
  param.transparent = transparent_;

#if defined(OS_POSIX)
  if (bitmaps_changed)
#endif
  {
    if (transport_store_.get())
      param.windowless_buffer = transport_store_->handle();

    if (background_store_.get())
      param.background_buffer = background_store_->handle();
  }

  IPC::Message* msg;
#if defined (OS_WIN)
  if (UseSynchronousGeometryUpdates()) {
    msg = new PluginMsg_UpdateGeometrySync(instance_id_, param);
  } else   
#endif
  {
    msg = new PluginMsg_UpdateGeometry(instance_id_, param);
    msg->set_unblock(true);
  }

  Send(msg);
}
