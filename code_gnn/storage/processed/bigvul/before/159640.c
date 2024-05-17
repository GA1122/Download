WebUIImpl* RenderFrameHostManager::GetNavigatingWebUI() const {
  if (speculative_render_frame_host_)
    return speculative_render_frame_host_->web_ui();
  return render_frame_host_->pending_web_ui();
}
