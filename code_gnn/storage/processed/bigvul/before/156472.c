void RenderFrameDevToolsAgentHost::ReadyToCommitNavigation(
    NavigationHandle* navigation_handle) {
  NavigationHandleImpl* handle =
      static_cast<NavigationHandleImpl*>(navigation_handle);
  for (auto* tracing : protocol::TracingHandler::ForAgentHost(this))
    tracing->ReadyToCommitNavigation(handle);

  if (handle->frame_tree_node() != frame_tree_node_) {
    if (ShouldForceCreation() && handle->GetRenderFrameHost() &&
        handle->GetRenderFrameHost()->IsCrossProcessSubframe()) {
      RenderFrameDevToolsAgentHost::GetOrCreateForDangling(
          handle->frame_tree_node());
    }
    return;
  }

  UpdateFrameHost(handle->GetRenderFrameHost());
}
