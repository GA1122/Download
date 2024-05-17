RenderFrameDevToolsAgentHost::RenderFrameDevToolsAgentHost(
    FrameTreeNode* frame_tree_node)
    : DevToolsAgentHostImpl(frame_tree_node->devtools_frame_token().ToString()),
      frame_tree_node_(frame_tree_node) {
  frame_host_ = frame_tree_node->current_frame_host();
  render_frame_alive_ = frame_host_ && frame_host_->IsRenderFrameLive();
  WebContentsObserver::Observe(
      WebContentsImpl::FromFrameTreeNode(frame_tree_node));

  g_agent_host_instances.Get().push_back(this);
  AddRef();   

  NotifyCreated();
}
