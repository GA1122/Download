bool RenderFrameHostImpl::ShouldDispatchBeforeUnload() {
  return IsRenderFrameLive();
}
