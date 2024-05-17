void RenderFrameHostManager::CancelPendingIfNecessary(
    RenderFrameHostImpl* render_frame_host) {
  if (render_frame_host == pending_render_frame_host_.get())
    CancelPending();
  else if (render_frame_host == speculative_render_frame_host_.get()) {
    if (frame_tree_node_->navigation_request() &&
        frame_tree_node_->navigation_request()->navigation_handle()) {
      frame_tree_node_->navigation_request()
          ->navigation_handle()
          ->set_net_error_code(net::ERR_ABORTED);
    }
    frame_tree_node_->ResetNavigationRequest(false, true);
  }
}
