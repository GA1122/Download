void RenderFrameDevToolsAgentHost::OnNavigationResponseReceived(
    const NavigationRequest& nav_request,
    const network::ResourceResponse& response) {
  FrameTreeNode* ftn = nav_request.frame_tree_node();
  std::string id = nav_request.devtools_navigation_token().ToString();
  std::string frame_id = ftn->devtools_frame_token().ToString();
  GURL url = nav_request.common_params().url;
  DispatchToAgents(ftn, &protocol::NetworkHandler::ResponseReceived, id, id,
                   url, protocol::Page::ResourceTypeEnum::Document,
                   response.head, frame_id);
}
