void RenderWidgetHostImpl::DragTargetDragEnterWithMetaData(
    const std::vector<DropData::Metadata>& metadata,
    const gfx::PointF& client_pt,
    const gfx::PointF& screen_pt,
    WebDragOperationsMask operations_allowed,
    int key_modifiers) {
  Send(new DragMsg_TargetDragEnter(GetRoutingID(), metadata, client_pt,
                                   screen_pt, operations_allowed,
                                   key_modifiers));
}
