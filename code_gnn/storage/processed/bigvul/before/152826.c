void MetricsWebContentsObserver::FrameSizeChanged(
    content::RenderFrameHost* render_frame_host,
    const gfx::Size& frame_size) {
  if (committed_load_)
    committed_load_->FrameSizeChanged(render_frame_host, frame_size);
}
