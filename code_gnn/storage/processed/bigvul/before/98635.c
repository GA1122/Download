void RenderWidget::DoDeferredUpdate() {
  if (!webwidget_ || !paint_aggregator_.HasPendingUpdate() ||
      update_reply_pending())
    return;

  if (is_hidden_ || size_.IsEmpty()) {
    paint_aggregator_.ClearPendingUpdate();
    needs_repainting_on_restore_ = true;
    return;
  }

  if (webwidget_->isAcceleratedCompositingActive() !=
      is_gpu_rendering_active_) {
    is_gpu_rendering_active_ = webwidget_->isAcceleratedCompositingActive();
    Send(new ViewHostMsg_GpuRenderingActivated(
        routing_id_, is_gpu_rendering_active_));
  }

  webwidget_->layout();

  PaintAggregator::PendingUpdate update = paint_aggregator_.GetPendingUpdate();
  paint_aggregator_.ClearPendingUpdate();

  gfx::Rect scroll_damage = update.GetScrollDamage();
  gfx::Rect bounds = update.GetPaintBounds().Union(scroll_damage);

  scoped_ptr<skia::PlatformCanvas> canvas(
      RenderProcess::current()->GetDrawingCanvas(&current_paint_buf_, bounds));
  if (!canvas.get()) {
    NOTREACHED();
    return;
  }

  DCHECK_EQ(bounds.width(), canvas->getDevice()->width());
  DCHECK_EQ(bounds.height(), canvas->getDevice()->height());
  bounds.set_width(canvas->getDevice()->width());
  bounds.set_height(canvas->getDevice()->height());

  HISTOGRAM_COUNTS_100("MPArch.RW_PaintRectCount", update.paint_rects.size());

  if (update.scroll_rect.IsEmpty()) {
    update.paint_rects.clear();
    update.paint_rects.push_back(bounds);
  }

  std::vector<gfx::Rect> copy_rects;
  copy_rects.swap(update.paint_rects);
  if (!scroll_damage.IsEmpty())
    copy_rects.push_back(scroll_damage);

  for (size_t i = 0; i < copy_rects.size(); ++i)
    PaintRect(copy_rects[i], bounds.origin(), canvas.get());

  ViewHostMsg_UpdateRect_Params params;
  params.bitmap = current_paint_buf_->id();
  params.bitmap_rect = bounds;
  params.dx = update.scroll_delta.x();
  params.dy = update.scroll_delta.y();
  if (is_gpu_rendering_active_) {
    params.scroll_rect = gfx::Rect();
    params.copy_rects.clear();
  } else {
    params.scroll_rect = update.scroll_rect;
    params.copy_rects.swap(copy_rects);   
  }
  params.view_size = size_;
  params.plugin_window_moves.swap(plugin_window_moves_);
  params.flags = next_paint_flags_;

  update_reply_pending_ = true;
  Send(new ViewHostMsg_UpdateRect(routing_id_, params));
  next_paint_flags_ = 0;

  UpdateIME();

  DidInitiatePaint();
}
