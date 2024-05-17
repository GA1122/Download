void RenderFrameDevToolsAgentHost::SetFrameTreeNode(
    FrameTreeNode* frame_tree_node) {
  if (frame_tree_node_)
    g_agent_host_instances.Get().erase(frame_tree_node_);
  frame_tree_node_ = frame_tree_node;
  if (frame_tree_node_) {
    g_agent_host_instances.Get()[frame_tree_node] = this;
  }
  WebContentsObserver::Observe(
      frame_tree_node_ ? WebContentsImpl::FromFrameTreeNode(frame_tree_node_)
                       : nullptr);
}
