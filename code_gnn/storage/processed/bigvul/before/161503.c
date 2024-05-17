void RenderFrameDevToolsAgentHost::FrameDeleted(RenderFrameHost* rfh) {
  if (static_cast<RenderFrameHostImpl*>(rfh)->frame_tree_node() ==
      frame_tree_node_) {
    DestroyOnRenderFrameGone();   
  }
}
