void RenderFrameDevToolsAgentHost::ConnectWebContents(WebContents* wc) {
  RenderFrameHostImpl* host =
      static_cast<RenderFrameHostImpl*>(wc->GetMainFrame());
  DCHECK(host);
  frame_tree_node_ = host->frame_tree_node();
  WebContentsObserver::Observe(wc);
  UpdateFrameHost(host);
}
