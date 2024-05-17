void RenderFrameHostImpl::OnExitFullscreen() {
  delegate_->ExitFullscreenMode(  true);

  render_view_host_->GetWidget()->SynchronizeVisualProperties();
}
