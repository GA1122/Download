void RenderViewImpl::FocusPrevious() {
  Send(new ViewHostMsg_TakeFocus(GetRoutingID(), true));
}
