RenderFrameDevToolsAgentHost* FindAgentHost(FrameTreeNode* frame_tree_node) {
  if (!g_agent_host_instances.IsCreated())
    return nullptr;
  for (RenderFrameDevToolsArray::iterator it =
           g_agent_host_instances.Get().begin();
       it != g_agent_host_instances.Get().end(); ++it) {
    if ((*it)->frame_tree_node() == frame_tree_node)
      return *it;
  }
  return nullptr;
}
