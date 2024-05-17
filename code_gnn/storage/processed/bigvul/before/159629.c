void RenderFrameHostManager::CreateOpenerProxiesForFrameTree(
    SiteInstance* instance,
    FrameTreeNode* skip_this_node) {
  DCHECK(frame_tree_node_->IsMainFrame());

  FrameTree* frame_tree = frame_tree_node_->frame_tree();

  if (skip_this_node && skip_this_node->frame_tree() != frame_tree)
    skip_this_node = nullptr;
  frame_tree->CreateProxiesForSiteInstance(skip_this_node, instance);
}
