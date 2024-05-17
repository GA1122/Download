void RenderWidgetHostImpl::ScrollFocusedEditableNodeIntoRect(
    const gfx::Rect& rect) {
  Send(new InputMsg_ScrollFocusedEditableNodeIntoRect(GetRoutingID(), rect));
}
