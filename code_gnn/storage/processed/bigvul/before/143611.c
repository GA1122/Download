void RenderWidgetHostImpl::DidUpdateBackingStore(
    const ViewHostMsg_UpdateRect_Params& params,
    const TimeTicks& paint_start) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::DidUpdateBackingStore");
  TimeTicks update_start = TimeTicks::Now();

  if (view_)
    view_->MovePluginWindows(params.plugin_window_moves);

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DID_UPDATE_BACKING_STORE,
      Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());

  if (is_hidden_)
    return;

  bool is_resize_ack =
      ViewHostMsg_UpdateRect_Flags::is_resize_ack(params.flags);
  if (is_resize_ack)
    WasResized();

  TimeTicks now = TimeTicks::Now();
  TimeDelta delta = now - update_start;
  UMA_HISTOGRAM_TIMES("MPArch.RWH_DidUpdateBackingStore", delta);
}
