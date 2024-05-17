RenderFrameHostManager::UnsetSpeculativeRenderFrameHost() {
  CHECK(IsBrowserSideNavigationEnabled());
  speculative_render_frame_host_->GetProcess()->RemovePendingView();
  return std::move(speculative_render_frame_host_);
}
