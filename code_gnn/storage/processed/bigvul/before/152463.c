void RenderFrameImpl::OnVisualStateRequest(uint64_t id) {
  GetLocalRootRenderWidget()->QueueMessage(
      new FrameHostMsg_VisualStateResponse(routing_id_, id));
}
