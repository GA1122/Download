void RenderFrameHostImpl::OnDidStartProvisionalLoad(
    const GURL& url,
    const std::vector<GURL>& redirect_chain,
    const base::TimeTicks& navigation_start) {
  if (!is_active())
    return;

  TRACE_EVENT2("navigation", "RenderFrameHostImpl::OnDidStartProvisionalLoad",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               url.possibly_invalid_spec());

  frame_tree_node_->navigator()->DidStartProvisionalLoad(
      this, url, redirect_chain, navigation_start);
}
