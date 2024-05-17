void RenderFrameHostImpl::OnToggleFullscreen(bool enter_fullscreen) {
  if (enter_fullscreen) {
    std::set<SiteInstance*> notified_instances;
    notified_instances.insert(GetSiteInstance());
    for (FrameTreeNode* node = frame_tree_node_; node->parent();
         node = node->parent()) {
      SiteInstance* parent_site_instance =
          node->parent()->current_frame_host()->GetSiteInstance();
      if (ContainsKey(notified_instances, parent_site_instance))
        continue;

      RenderFrameProxyHost* child_proxy =
          node->render_manager()->GetRenderFrameProxyHost(parent_site_instance);
      child_proxy->Send(
          new FrameMsg_WillEnterFullscreen(child_proxy->GetRoutingID()));
      notified_instances.insert(parent_site_instance);
    }
  }

  if (enter_fullscreen)
    delegate_->EnterFullscreenMode(GetLastCommittedURL().GetOrigin());
  else
    delegate_->ExitFullscreenMode(  true);

  render_view_host_->GetWidget()->WasResized();
}