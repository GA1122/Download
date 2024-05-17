void RenderViewHostImpl::SendOrientationChangeEvent(int orientation) {
  Send(new ViewMsg_OrientationChangeEvent(GetRoutingID(), orientation));
}
