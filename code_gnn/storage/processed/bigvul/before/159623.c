void RenderFrameHostManager::CleanUpNavigation() {
  if (speculative_render_frame_host_) {
    if (speculative_render_frame_host_->navigation_handle()) {
      frame_tree_node_->navigator()->DiscardPendingEntryIfNeeded(
          speculative_render_frame_host_->navigation_handle()
              ->pending_nav_entry_id());
    }
    bool was_loading = speculative_render_frame_host_->is_loading();
    DiscardUnusedFrame(UnsetSpeculativeRenderFrameHost());
    if (was_loading)
      frame_tree_node_->DidStopLoading();
  }
}
