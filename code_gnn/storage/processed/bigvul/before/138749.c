void RenderFrameHostImpl::OnDidStartProvisionalLoad(
    const GURL& url,
    const std::vector<GURL>& redirect_chain,
    const base::TimeTicks& navigation_start) {
  if (!is_active())
    return;
  frame_tree_node_->navigator()->DidStartProvisionalLoad(
      this, url, redirect_chain, navigation_start);
}
