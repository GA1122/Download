RenderFrameHostManager::~RenderFrameHostManager() {
  if (speculative_render_frame_host_)
    UnsetSpeculativeRenderFrameHost();

  ResetProxyHosts();

  SetRenderFrameHost(std::unique_ptr<RenderFrameHostImpl>());
}
