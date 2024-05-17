void RenderWidgetHostImpl::OnUpdateRect(
    const ViewHostMsg_UpdateRect_Params& params) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::OnUpdateRect");
  TimeTicks paint_start = TimeTicks::Now();

  current_size_ = params.view_size;

  bool is_resize_ack =
      ViewHostMsg_UpdateRect_Flags::is_resize_ack(params.flags);

  if (is_resize_ack) {
    DCHECK(!g_check_for_pending_resize_ack || resize_ack_pending_);
    resize_ack_pending_ = false;
  }

  bool is_repaint_ack =
      ViewHostMsg_UpdateRect_Flags::is_repaint_ack(params.flags);
  if (is_repaint_ack) {
    DCHECK(repaint_ack_pending_);
    TRACE_EVENT_ASYNC_END0(
        "renderer_host", "RenderWidgetHostImpl::repaint_ack_pending_", this);
    repaint_ack_pending_ = false;
    TimeDelta delta = TimeTicks::Now() - repaint_start_time_;
    UMA_HISTOGRAM_TIMES("MPArch.RWH_RepaintDelta", delta);
  }

  DCHECK(!params.view_size.IsEmpty());

  DidUpdateBackingStore(params, paint_start);

  if (auto_resize_enabled_) {
    bool post_callback = new_auto_size_.IsEmpty();
    new_auto_size_ = params.view_size;
    if (post_callback) {
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::Bind(&RenderWidgetHostImpl::DelayedAutoResized,
                                weak_factory_.GetWeakPtr()));
    }
  }

  TimeDelta delta = TimeTicks::Now() - paint_start;
  UMA_HISTOGRAM_TIMES("MPArch.RWH_OnMsgUpdateRect", delta);
}
