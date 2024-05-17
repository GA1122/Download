void RenderFrameDevToolsAgentHost::DisconnectWebContents() {
  frame_tree_node_ = nullptr;
  navigation_handles_.clear();
  WebContentsObserver::Observe(nullptr);
  scoped_refptr<RenderFrameDevToolsAgentHost> protect(this);
  UpdateFrameHost(nullptr);
  for (DevToolsSession* session : sessions())
    session->ResumeSendingMessagesToAgent();
}
