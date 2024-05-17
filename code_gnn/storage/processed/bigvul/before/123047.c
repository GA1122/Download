void RenderWidgetHostImpl::ScrollFocusedEditableNodeIntoRect(
    const gfx::Rect& rect) {
  Send(new ViewMsg_ScrollFocusedEditableNodeIntoRect(GetRoutingID(), rect));
}
