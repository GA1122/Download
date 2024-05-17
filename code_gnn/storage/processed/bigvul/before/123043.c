void RenderWidgetHostImpl::ResizeRectChanged(const gfx::Rect& new_rect) {
  Send(new ViewMsg_ChangeResizeRect(routing_id_, new_rect));
}
