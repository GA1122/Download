void RenderWidgetHostImpl::Stop() {
  Send(new ViewMsg_Stop(GetRoutingID()));
}
