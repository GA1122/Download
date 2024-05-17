void RenderFrameHostManager::Stop() {
  render_frame_host_->Stop();

  if (speculative_render_frame_host_ &&
      speculative_render_frame_host_->is_loading()) {
    speculative_render_frame_host_->Send(
        new FrameMsg_Stop(speculative_render_frame_host_->GetRoutingID()));
  }
}
