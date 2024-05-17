void RenderViewHostImpl::SetZoomLevel(double level) {
  Send(new ViewMsg_SetZoomLevel(GetRoutingID(), level));
}
