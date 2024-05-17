void RenderWidgetHostImpl::MoveCaret(const gfx::Point& point) {
  Send(new InputMsg_MoveCaret(GetRoutingID(), point));
}
