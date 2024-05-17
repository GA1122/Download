void RenderFrameHostManager::InitializeRenderFrameIfNecessary(
    RenderFrameHostImpl* render_frame_host) {
  if (render_frame_host->IsRenderFrameLive())
    return;

  if (!ReinitializeRenderFrame(render_frame_host))
    return;

  if (render_frame_host != render_frame_host_.get())
    return;

  EnsureRenderFrameHostVisibilityConsistent();


  delegate_->NotifyMainFrameSwappedFromRenderManager(
      nullptr, render_frame_host_->render_view_host());
}
