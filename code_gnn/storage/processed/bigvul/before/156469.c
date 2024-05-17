void RenderFrameDevToolsAgentHost::OnSignedExchangeCertificateResponseReceived(
    FrameTreeNode* frame_tree_node,
    const base::UnguessableToken& request_id,
    const base::UnguessableToken& loader_id,
    const GURL& url,
    const network::ResourceResponseHead& head) {
  DispatchToAgents(frame_tree_node, &protocol::NetworkHandler::ResponseReceived,
                   request_id.ToString(), loader_id.ToString(), url,
                   protocol::Network::ResourceTypeEnum::Other, head,
                   protocol::Maybe<std::string>());
}
