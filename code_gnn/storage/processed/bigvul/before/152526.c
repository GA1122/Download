void RenderFrameImpl::SetNeedsOcclusionTracking(bool needs_tracking) {
  Send(new FrameHostMsg_SetNeedsOcclusionTracking(routing_id_, needs_tracking));
}
