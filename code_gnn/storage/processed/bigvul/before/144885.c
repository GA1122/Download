void RenderWidgetHostViewAura::ClearCompositorFrame() {
  if (delegated_frame_host_)
    delegated_frame_host_->ClearDelegatedFrame();
}
