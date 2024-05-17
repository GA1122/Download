void RenderWidgetHostImpl::OnMsgUpdateRect(
    const ViewHostMsg_UpdateRect_Params& params) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::OnMsgUpdateRect");
  TimeTicks paint_start = TimeTicks::Now();

  current_size_ = params.view_size;
  last_scroll_offset_ = params.scroll_offset;

  bool is_resize_ack =
      ViewHostMsg_UpdateRect_Flags::is_resize_ack(params.flags);

  if (is_resize_ack) {
    DCHECK(resize_ack_pending_);
    resize_ack_pending_ = false;
    in_flight_size_.SetSize(0, 0);
  }

  bool is_repaint_ack =
      ViewHostMsg_UpdateRect_Flags::is_repaint_ack(params.flags);
  if (is_repaint_ack) {
    repaint_ack_pending_ = false;
    TimeDelta delta = TimeTicks::Now() - repaint_start_time_;
    UMA_HISTOGRAM_TIMES("MPArch.RWH_RepaintDelta", delta);
  }

  DCHECK(!params.view_size.IsEmpty());

  bool was_async = false;

  TransportDIB* dib = process_->GetTransportDIB(params.bitmap);

  if (dib) {
    DCHECK(!params.bitmap_rect.IsEmpty());
    gfx::Size pixel_size = gfx::ToFlooredSize(
        gfx::ScaleSize(params.bitmap_rect.size(), params.scale_factor));
    const size_t size = pixel_size.height() * pixel_size.width() * 4;
    if (dib->size() < size) {
      DLOG(WARNING) << "Transport DIB too small for given rectangle";
      RecordAction(UserMetricsAction("BadMessageTerminate_RWH1"));
      GetProcess()->ReceivedBadMessage();
    } else {
      UNSHIPPED_TRACE_EVENT_INSTANT2("test_latency", "UpdateRect",
          "x+y", params.bitmap_rect.x() + params.bitmap_rect.y(),
          "color", 0xffffff & *static_cast<uint32*>(dib->memory()));
      UNSHIPPED_TRACE_EVENT_INSTANT1("test_latency", "UpdateRectWidth",
          "width", params.bitmap_rect.width());

      if (!params.scroll_rect.IsEmpty()) {
        ScrollBackingStoreRect(params.scroll_delta,
                               params.scroll_rect,
                               params.view_size);
      }

      was_async = PaintBackingStoreRect(
          params.bitmap,
          params.bitmap_rect,
          params.copy_rects,
          params.view_size,
          params.scale_factor,
          base::Bind(&RenderWidgetHostImpl::DidUpdateBackingStore,
                     weak_factory_.GetWeakPtr(), params, paint_start));
    }
  }

  if (!was_async) {
    DidUpdateBackingStore(params, paint_start);
  }

  if (should_auto_resize_) {
    bool post_callback = new_auto_size_.IsEmpty();
    new_auto_size_ = params.view_size;
    if (post_callback) {
      MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&RenderWidgetHostImpl::DelayedAutoResized,
                     weak_factory_.GetWeakPtr()));
    }
  }

  TimeDelta delta = TimeTicks::Now() - paint_start;
  UMA_HISTOGRAM_TIMES("MPArch.RWH_OnMsgUpdateRect", delta);
}
