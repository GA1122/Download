void RenderWidgetHostImpl::DragTargetDragLeave(
    const gfx::PointF& client_point,
    const gfx::PointF& screen_point) {
  Send(new DragMsg_TargetDragLeave(GetRoutingID(), client_point, screen_point));
}
