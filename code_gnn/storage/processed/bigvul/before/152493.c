void RenderFrameImpl::RenderFallbackContentInParentProcess() {
  Send(new FrameHostMsg_RenderFallbackContentInParentProcess(routing_id_));
}
