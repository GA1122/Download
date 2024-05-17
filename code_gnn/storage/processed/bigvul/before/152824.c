void MetricsWebContentsObserver::FrameDisplayStateChanged(
    content::RenderFrameHost* render_frame_host,
    bool is_display_none) {
  if (committed_load_)
    committed_load_->FrameDisplayStateChanged(render_frame_host,
                                              is_display_none);
}
