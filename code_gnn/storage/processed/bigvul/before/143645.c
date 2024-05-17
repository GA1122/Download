void RenderWidgetHostImpl::HandleCompositorProto(
    const std::vector<uint8_t>& proto) {
  DCHECK(!proto.empty());
  Send(new ViewMsg_HandleCompositorProto(GetRoutingID(), proto));
}
