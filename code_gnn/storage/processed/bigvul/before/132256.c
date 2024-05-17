void RenderFrameImpl::OnVisualStateRequest(uint64 id) {
  GetRenderWidget()->QueueMessage(
      new FrameHostMsg_VisualStateResponse(routing_id_, id),
      MESSAGE_DELIVERY_POLICY_WITH_VISUAL_STATE);
}
