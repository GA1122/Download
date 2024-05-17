void RenderWidgetHostImpl::ShowContextMenuAtPoint(const gfx::Point& point) {
  Send(new ViewMsg_ShowContextMenu(
      GetRoutingID(), ui::MENU_SOURCE_MOUSE, point));
}
