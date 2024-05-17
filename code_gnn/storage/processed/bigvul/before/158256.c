void RenderWidgetHostImpl::DragTargetDragOver(
    const gfx::PointF& client_pt,
    const gfx::PointF& screen_pt,
    WebDragOperationsMask operations_allowed,
    int key_modifiers) {
  Send(new DragMsg_TargetDragOver(GetRoutingID(), client_pt, screen_pt,
                                  operations_allowed, key_modifiers));
}
