void RenderFrameHostManager::SetIsLoading(bool is_loading) {
  render_frame_host_->render_view_host()->GetWidget()->SetIsLoading(is_loading);
  if (pending_render_frame_host_) {
    pending_render_frame_host_->render_view_host()->GetWidget()->SetIsLoading(
        is_loading);
  }
}
