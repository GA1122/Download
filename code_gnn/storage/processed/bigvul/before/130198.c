void RenderFrameHostImpl::SwapOut() {
  if (render_view_host_->IsRenderViewLive()) {
    Send(new FrameMsg_SwapOut(routing_id_));
  } else {
    OnSwappedOut(true);
  }
}
