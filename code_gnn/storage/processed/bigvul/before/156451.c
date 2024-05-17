RenderFrameDevToolsAgentHost::GetOrCreateForDangling(
    FrameTreeNode* frame_tree_node) {
  RenderFrameDevToolsAgentHost* result = FindAgentHost(frame_tree_node);
  if (!result)
    result = new RenderFrameDevToolsAgentHost(frame_tree_node);
  return result;
}
