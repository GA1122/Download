void RenderViewHostImpl::Zoom(PageZoom zoom) {
  Send(new ViewMsg_Zoom(GetRoutingID(), zoom));
}
