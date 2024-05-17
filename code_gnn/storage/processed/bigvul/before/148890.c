bool RenderFrameHostManager::InitRenderFrame(
    RenderFrameHostImpl* render_frame_host) {
  if (render_frame_host->IsRenderFrameLive())
    return true;

  SiteInstance* site_instance = render_frame_host->GetSiteInstance();

  int opener_routing_id = MSG_ROUTING_NONE;
  if (frame_tree_node_->opener())
    opener_routing_id = GetOpenerRoutingID(site_instance);

  int parent_routing_id = MSG_ROUTING_NONE;
  if (frame_tree_node_->parent()) {
    parent_routing_id = frame_tree_node_->parent()
                            ->render_manager()
                            ->GetRoutingIdForSiteInstance(site_instance);
    CHECK_NE(parent_routing_id, MSG_ROUTING_NONE);
  }

  int previous_sibling_routing_id = MSG_ROUTING_NONE;
  FrameTreeNode* previous_sibling = frame_tree_node_->PreviousSibling();
  if (previous_sibling) {
    previous_sibling_routing_id =
        previous_sibling->render_manager()->GetRoutingIdForSiteInstance(
            site_instance);
    CHECK_NE(previous_sibling_routing_id, MSG_ROUTING_NONE);
  }

  int proxy_routing_id = MSG_ROUTING_NONE;
  RenderFrameProxyHost* existing_proxy = GetRenderFrameProxyHost(site_instance);
  if (existing_proxy) {
    proxy_routing_id = existing_proxy->GetRoutingID();
    CHECK_NE(proxy_routing_id, MSG_ROUTING_NONE);
    if (!existing_proxy->is_render_frame_proxy_live())
      existing_proxy->InitRenderFrameProxy();
  }

  if (!existing_proxy && frame_tree_node_->parent()) {
    RenderFrameProxyHost* parent_proxy = RenderFrameProxyHost::FromID(
        render_frame_host->GetProcess()->GetID(), parent_routing_id);
    if (!parent_proxy || !parent_proxy->is_render_frame_proxy_live()) {
      base::debug::SetCrashKeyValue("initrf_parent_proxy_exists",
                                    parent_proxy ? "yes" : "no");

      SiteInstance* parent_instance =
          frame_tree_node_->parent()->current_frame_host()->GetSiteInstance();
      base::debug::SetCrashKeyValue(
          "initrf_parent_is_in_same_site_instance",
          site_instance == parent_instance ? "yes" : "no");
      base::debug::SetCrashKeyValue("initrf_parent_process_is_live",
                                    frame_tree_node_->parent()
                                            ->current_frame_host()
                                            ->GetProcess()
                                            ->HasConnection()
                                        ? "yes"
                                        : "no");
      base::debug::SetCrashKeyValue(
          "initrf_render_view_is_live",
          render_frame_host->render_view_host()->IsRenderViewLive() ? "yes"
                                                                    : "no");

      FrameTreeNode* root = frame_tree_node_->frame_tree()->root();
      if (root != frame_tree_node_->parent()) {
        SiteInstance* root_instance =
            root->current_frame_host()->GetSiteInstance();
        base::debug::SetCrashKeyValue(
            "initrf_root_is_in_same_site_instance",
            site_instance == root_instance ? "yes" : "no");
        base::debug::SetCrashKeyValue(
            "initrf_root_is_in_same_site_instance_as_parent",
            parent_instance == root_instance ? "yes" : "no");
        base::debug::SetCrashKeyValue("initrf_root_process_is_live",
                                      frame_tree_node_->frame_tree()
                                              ->root()
                                              ->current_frame_host()
                                              ->GetProcess()
                                              ->HasConnection()
                                          ? "yes"
                                          : "no");

        RenderFrameProxyHost* top_proxy =
            root->render_manager()->GetRenderFrameProxyHost(site_instance);
        if (top_proxy) {
          base::debug::SetCrashKeyValue(
              "initrf_root_proxy_is_live",
              top_proxy->is_render_frame_proxy_live() ? "yes" : "no");
        }
      }

      base::debug::DumpWithoutCrashing();
    }
  }

  return delegate_->CreateRenderFrameForRenderManager(
      render_frame_host, proxy_routing_id, opener_routing_id, parent_routing_id,
      previous_sibling_routing_id);
}
