void RenderWidgetHostImpl::DonePaintingToBackingStore() {
  Send(new ViewMsg_UpdateRect_ACK(GetRoutingID()));
}
