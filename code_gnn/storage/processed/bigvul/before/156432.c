RenderFrameDevToolsAgentHost::CreateNavigationThrottles(
    NavigationHandleImpl* navigation_handle) {
  std::vector<std::unique_ptr<NavigationThrottle>> result;
  FrameTreeNode* frame_tree_node = navigation_handle->frame_tree_node();

  RenderFrameDevToolsAgentHost* agent_host = FindAgentHost(frame_tree_node);
  if (agent_host) {
    for (auto* network_handler :
         protocol::NetworkHandler::ForAgentHost(agent_host)) {
      std::unique_ptr<NavigationThrottle> throttle =
          network_handler->CreateThrottleForNavigation(navigation_handle);
      if (throttle)
        result.push_back(std::move(throttle));
    }
  }

  agent_host = nullptr;
  if (frame_tree_node->parent()) {
    agent_host =
        FindAgentHost(GetFrameTreeNodeAncestor(frame_tree_node->parent()));
  }
  if (agent_host) {
    for (auto* target_handler :
         protocol::TargetHandler::ForAgentHost(agent_host)) {
      std::unique_ptr<NavigationThrottle> throttle =
          target_handler->CreateThrottleForNavigation(navigation_handle);
      if (throttle)
        result.push_back(std::move(throttle));
    }
  }

  return result;
}
