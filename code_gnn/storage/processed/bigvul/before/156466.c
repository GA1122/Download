void RenderFrameDevToolsAgentHost::OnResetNavigationRequest(
    NavigationRequest* navigation_request) {
  for (FrameTreeNode* node = navigation_request->frame_tree_node(); node;
       node = node->parent()) {
    DispatchToAgents(node, &protocol::PageHandler::NavigationReset,
                     navigation_request);
  }
}
