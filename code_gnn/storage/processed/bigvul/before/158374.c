void RenderWidgetHostImpl::ShowContextMenuAtPoint(
    const gfx::Point& point,
    const ui::MenuSourceType source_type) {
  Send(new ViewMsg_ShowContextMenu(GetRoutingID(), source_type, point));
}
