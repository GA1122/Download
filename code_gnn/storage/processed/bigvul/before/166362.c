DevToolsAgentHostImpl* RenderFrameDevToolsAgentHost::GetFor(
    FrameTreeNode* frame_tree_node) {
  frame_tree_node = GetFrameTreeNodeAncestor(frame_tree_node);
  return FindAgentHost(frame_tree_node);
}
