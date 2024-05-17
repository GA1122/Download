void RenderFrameDevToolsAgentHost::DestroyOnRenderFrameGone() {
  scoped_refptr<RenderFrameDevToolsAgentHost> protect(this);
  if (IsAttached())
    RevokePolicy();
  ForceDetachAllSessions();
  frame_host_ = nullptr;
  UpdateRendererChannel(IsAttached());
  SetFrameTreeNode(nullptr);
  Release();
}
