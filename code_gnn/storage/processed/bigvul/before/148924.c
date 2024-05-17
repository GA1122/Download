RenderViewHostImpl* RenderFrameHostManager::pending_render_view_host() const {
  if (!pending_render_frame_host_)
    return nullptr;
  return pending_render_frame_host_->render_view_host();
}
