void RenderFrameDevToolsAgentHost::OnNavigationRequestWillBeSent(
    const NavigationRequest& navigation_request) {
  DispatchToAgents(navigation_request.frame_tree_node(),
                   &protocol::NetworkHandler::NavigationRequestWillBeSent,
                   navigation_request);
}
