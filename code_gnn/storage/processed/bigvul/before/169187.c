RenderFrameHostImpl::~RenderFrameHostImpl() {
  ResetNavigationRequests();

  ClearAllWebUI();

  SetLastCommittedSiteUrl(GURL());

  GetProcess()->RemoveRoute(routing_id_);
  g_routing_id_frame_map.Get().erase(
      RenderFrameHostID(GetProcess()->GetID(), routing_id_));

  if (overlay_routing_token_)
    g_token_frame_map.Get().erase(*overlay_routing_token_);

  site_instance_->RemoveObserver(this);

  if (delegate_ && render_frame_created_)
    delegate_->RenderFrameDeleted(this);

  GetSiteInstance()->DecrementActiveFrameCount();

  bool will_render_view_clean_up_render_frame =
      frame_tree_node_->IsMainFrame() && render_view_host_->ref_count() == 1;
  if (is_active() && render_frame_created_ &&
      !will_render_view_clean_up_render_frame) {
    Send(new FrameMsg_Delete(routing_id_));
  }

  swapout_event_monitor_timeout_.reset();

  for (const auto& iter : visual_state_callbacks_)
    iter.second.Run(false);

  if (render_widget_host_ &&
      render_widget_host_->owned_by_render_frame_host()) {
    render_widget_host_->ShutdownAndDestroyWidget(true);
  }

  frame_tree_->ReleaseRenderViewHostRef(render_view_host_);

  ui::AXTreeIDRegistry::GetInstance()->RemoveAXTreeID(ax_tree_id_);
}
