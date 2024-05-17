void RenderFrameDevToolsAgentHost::DidStartNavigation(
    NavigationHandle* navigation_handle) {
  NavigationHandleImpl* handle =
      static_cast<NavigationHandleImpl*>(navigation_handle);
  if (handle->frame_tree_node() != frame_tree_node_)
    return;
  if (navigation_handles_.empty()) {
    for (DevToolsSession* session : sessions())
      session->SuspendSendingMessagesToAgent();
  }
  navigation_handles_.insert(handle);
}
