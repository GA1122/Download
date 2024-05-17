void RenderFrameHostManager::CleanUpNavigation() {
  CHECK(IsBrowserSideNavigationEnabled());
  if (speculative_render_frame_host_) {
    bool was_loading = speculative_render_frame_host_->is_loading();
    DiscardUnusedFrame(UnsetSpeculativeRenderFrameHost());
    if (was_loading)
      frame_tree_node_->DidStopLoading();
  }
}
