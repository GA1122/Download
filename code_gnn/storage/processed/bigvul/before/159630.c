void RenderFrameHostManager::CreateProxiesForNewNamedFrame() {
  DCHECK(!frame_tree_node_->frame_name().empty());

  FrameTreeNode* opener = frame_tree_node_->opener();
  if (!opener || !frame_tree_node_->IsMainFrame())
    return;
  SiteInstance* current_instance = render_frame_host_->GetSiteInstance();

  for (FrameTreeNode* ancestor = opener->parent(); ancestor;
       ancestor = ancestor->parent()) {
    RenderFrameHostImpl* ancestor_rfh = ancestor->current_frame_host();
    if (ancestor_rfh->GetSiteInstance() != current_instance)
      CreateRenderFrameProxy(ancestor_rfh->GetSiteInstance());
  }
}
