void RenderFrameHostManager::Stop() {
  render_frame_host_->Stop();

  if (pending_render_frame_host_) {
    pending_render_frame_host_->Send(new FrameMsg_Stop(
        pending_render_frame_host_->GetRoutingID()));
  }

  if (IsBrowserSideNavigationEnabled()) {
    if (speculative_render_frame_host_ &&
        speculative_render_frame_host_->is_loading()) {
      speculative_render_frame_host_->Send(
          new FrameMsg_Stop(speculative_render_frame_host_->GetRoutingID()));
    }
  }
}
