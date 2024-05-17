bool RenderFrameDevToolsAgentHost::WillCreateURLLoaderFactory(
    RenderFrameHostImpl* rfh,
    bool is_navigation,
    bool is_download,
    network::mojom::URLLoaderFactoryRequest* target_factory_request) {
  FrameTreeNode* frame_tree_node = rfh->frame_tree_node();
  base::UnguessableToken frame_token = frame_tree_node->devtools_frame_token();
  frame_tree_node = GetFrameTreeNodeAncestor(frame_tree_node);
  RenderFrameDevToolsAgentHost* agent_host = FindAgentHost(frame_tree_node);
  if (!agent_host)
    return false;
  int process_id = is_navigation ? 0 : rfh->GetProcess()->GetID();
  DCHECK(!is_download || is_navigation);
  for (auto* network : protocol::NetworkHandler::ForAgentHost(agent_host)) {
    if (network->MaybeCreateProxyForInterception(
            frame_token, process_id, is_download, target_factory_request)) {
      return true;
    }
  }
  return false;
}
