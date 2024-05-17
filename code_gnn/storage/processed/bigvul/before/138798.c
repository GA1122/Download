void RenderFrameHostImpl::SendNavigateMessage(
    const CommonNavigationParams& common_params,
    const StartNavigationParams& start_params,
    const RequestNavigationParams& request_params) {
  RenderFrameDevToolsAgentHost::OnBeforeNavigation(
      frame_tree_node_->current_frame_host(), this);
  Send(new FrameMsg_Navigate(
      routing_id_, common_params, start_params, request_params));
}
