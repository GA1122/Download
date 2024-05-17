void RenderFrameDevToolsAgentHost::OnResetNavigationRequest(
    NavigationRequest* navigation_request) {
  RenderFrameDevToolsAgentHost* agent_host =
      FindAgentHost(navigation_request->frame_tree_node());
  if (!agent_host)
    return;
  if (navigation_request->net_error() != net::OK) {
    for (auto* network : protocol::NetworkHandler::ForAgentHost(agent_host))
      network->NavigationFailed(navigation_request);
  }
  for (auto* page : protocol::PageHandler::ForAgentHost(agent_host))
    page->NavigationReset(navigation_request);
}
