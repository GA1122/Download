void RenderWidgetHostImpl::DidCompleteResizeOrRepaint(
    const ViewHostMsg_ResizeOrRepaint_ACK_Params& params,
    const TimeTicks& paint_start) {
  TRACE_EVENT0("renderer_host",
               "RenderWidgetHostImpl::DidCompleteResizeOrRepaint");

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DID_COMPLETE_RESIZE_OR_REPAINT,
      Source<RenderWidgetHost>(this), NotificationService::NoDetails());

  if (is_hidden_)
    return;

  bool is_resize_ack =
      ViewHostMsg_ResizeOrRepaint_ACK_Flags::is_resize_ack(params.flags);
  if (is_resize_ack)
    WasResized();
}
