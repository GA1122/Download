void RenderFrameHostManager::CreateOpenerProxies(
    SiteInstance* instance,
    FrameTreeNode* skip_this_node) {
  std::vector<FrameTree*> opener_frame_trees;
  base::hash_set<FrameTreeNode*> nodes_with_back_links;

  CollectOpenerFrameTrees(&opener_frame_trees, &nodes_with_back_links);

  for (int i = opener_frame_trees.size() - 1; i >= 0; i--) {
    opener_frame_trees[i]
        ->root()
        ->render_manager()
        ->CreateOpenerProxiesForFrameTree(instance, skip_this_node);
  }

  for (auto* node : nodes_with_back_links) {
    RenderFrameProxyHost* proxy =
        node->render_manager()->GetRenderFrameProxyHost(instance);
    if (!proxy)
      continue;

    int opener_routing_id =
        node->render_manager()->GetOpenerRoutingID(instance);
    DCHECK_NE(opener_routing_id, MSG_ROUTING_NONE);
    proxy->Send(new FrameMsg_UpdateOpener(proxy->GetRoutingID(),
                                          opener_routing_id));
  }
}
