void RenderFrameHostManager::CreateOpenerProxiesForFrameTree(
    SiteInstance* instance,
    FrameTreeNode* skip_this_node) {
  DCHECK(frame_tree_node_->IsMainFrame());

  if (frame_tree_node_ == skip_this_node)
    return;

  FrameTree* frame_tree = frame_tree_node_->frame_tree();
  if (SiteIsolationPolicy::AreCrossProcessFramesPossible()) {
    if (skip_this_node && skip_this_node->frame_tree() != frame_tree)
      skip_this_node = nullptr;
    frame_tree->CreateProxiesForSiteInstance(skip_this_node, instance);
  } else {
    RenderViewHostImpl* rvh = frame_tree->GetRenderViewHost(instance);
    bool need_proxy_for_pending_rvh = (rvh == pending_render_view_host());
    bool need_proxy_for_speculative_rvh =
        IsBrowserSideNavigationEnabled() && speculative_render_frame_host_ &&
        speculative_render_frame_host_->GetRenderViewHost() == rvh;
    if (rvh && rvh->IsRenderViewLive() && !need_proxy_for_pending_rvh &&
        !need_proxy_for_speculative_rvh) {
      return;
    }

    if (rvh && !rvh->IsRenderViewLive()) {
      EnsureRenderViewInitialized(rvh, instance);
    } else {
      frame_tree->root()->render_manager()->CreateRenderFrameProxy(instance);
    }
  }
}
