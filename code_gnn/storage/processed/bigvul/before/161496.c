void RenderFrameDevToolsAgentHost::DestroyOnRenderFrameGone() {
  scoped_refptr<RenderFrameDevToolsAgentHost> protect(this);
  if (IsAttached())
    RevokePolicy();
  ForceDetachAllClients();
  frame_host_ = nullptr;
  agent_ptr_.reset();
  frame_tree_node_ = nullptr;
  WebContentsObserver::Observe(nullptr);
  Release();
}
