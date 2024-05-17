void MetricsWebContentsObserver::FrameReceivedFirstUserActivation(
    content::RenderFrameHost* render_frame_host) {
  if (committed_load_)
    committed_load_->FrameReceivedFirstUserActivation(render_frame_host);
}
