void RenderViewImpl::FocusNext() {
  Send(new ViewHostMsg_TakeFocus(GetRoutingID(), false));
}
