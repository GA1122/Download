void WebPluginDelegateProxy::Paint(WebKit::WebCanvas* canvas,
                                   const gfx::Rect& damaged_rect) {
  gfx::Rect rect = damaged_rect.Intersect(plugin_rect_);

  if (!channel_host_ || !channel_host_->channel_valid()) {
    PaintSadPlugin(canvas, rect);
    return;
  }

  if (!windowless_)
    return;

  if (!backing_store_canvas_.get())
    return;

#if WEBKIT_USING_SKIA
  gfx::NativeDrawingContext context = canvas->beginPlatformPaint();
#elif WEBKIT_USING_CG
  gfx::NativeDrawingContext context = canvas;
#endif

  gfx::Rect offset_rect = rect;
  offset_rect.Offset(-plugin_rect_.x(), -plugin_rect_.y());
  gfx::Rect canvas_rect = offset_rect;
#if defined(OS_MACOSX)
  FlipRectVerticallyWithHeight(&canvas_rect, plugin_rect_.height());
#endif

  bool background_changed = false;
  if (background_store_canvas_.get() && BackgroundChanged(context, rect)) {
    background_changed = true;
    gfx::Rect flipped_offset_rect = offset_rect;
    BlitContextToCanvas(background_store_canvas_.get(), canvas_rect,
                        context, rect.origin());
  }

  if (background_changed || !backing_store_painted_.Contains(offset_rect)) {
    Send(new PluginMsg_Paint(instance_id_, offset_rect));
    CopyFromTransportToBacking(offset_rect);
  }

  BlitCanvasToContext(context, rect, backing_store_canvas_.get(),
                      canvas_rect.origin());

  if (invalidate_pending_) {
    invalidate_pending_ = false;
    Send(new PluginMsg_DidPaint(instance_id_));
  }

#if WEBKIT_USING_SKIA
  canvas->endPlatformPaint();
#endif
}
