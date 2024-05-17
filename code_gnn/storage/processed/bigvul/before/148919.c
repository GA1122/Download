RenderFrameHostManager::UnsetPendingRenderFrameHost() {
  std::unique_ptr<RenderFrameHostImpl> pending_render_frame_host =
      std::move(pending_render_frame_host_);

  RenderFrameDevToolsAgentHost::OnCancelPendingNavigation(
      pending_render_frame_host.get(),
      render_frame_host_.get());

  pending_render_frame_host->GetProcess()->RemovePendingView();

  return pending_render_frame_host;
}
