void RenderWidgetHostImpl::DragSourceEndedAt(
    const gfx::PointF& client_pt,
    const gfx::PointF& screen_pt,
    blink::WebDragOperation operation) {
  Send(new DragMsg_SourceEnded(GetRoutingID(),
                               client_pt,
                               screen_pt,
                               operation));
}
