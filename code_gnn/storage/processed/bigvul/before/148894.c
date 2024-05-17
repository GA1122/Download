RenderFrameHostImpl* RenderFrameHostManager::Navigate(
    const GURL& dest_url,
    const FrameNavigationEntry& frame_entry,
    const NavigationEntryImpl& entry,
    bool is_reload) {
  TRACE_EVENT1("navigation", "RenderFrameHostManager:Navigate",
               "FrameTreeNode id", frame_tree_node_->frame_tree_node_id());
  RenderFrameHostImpl* dest_render_frame_host = UpdateStateForNavigate(
      dest_url, frame_entry.source_site_instance(), frame_entry.site_instance(),
      entry.GetTransitionType(), entry.restore_type() != RestoreType::NONE,
      entry.IsViewSourceMode(), entry.transferred_global_request_id(),
      entry.bindings(), is_reload);
  if (!dest_render_frame_host)
    return nullptr;   

  if (!dest_render_frame_host->IsRenderFrameLive()) {
    dest_render_frame_host->SetUpMojoIfNeeded();

    if (!ReinitializeRenderFrame(dest_render_frame_host))
      return nullptr;

    if (GetNavigatingWebUI()) {
      GetNavigatingWebUI()->RenderFrameCreated(dest_render_frame_host);
    }

    if (dest_render_frame_host != render_frame_host_.get()) {
      if (dest_render_frame_host->GetView())
        dest_render_frame_host->GetView()->Hide();
    } else {
      EnsureRenderFrameHostVisibilityConsistent();

      delegate_->NotifyMainFrameSwappedFromRenderManager(
          nullptr, render_frame_host_->render_view_host());
    }
  }

  if (transfer_navigation_handle_.get() &&
      transfer_navigation_handle_->GetGlobalRequestID() ==
          entry.transferred_global_request_id()) {
    dest_render_frame_host->SetNavigationHandle(
        std::move(transfer_navigation_handle_));

    dest_render_frame_host->navigation_handle()->set_render_frame_host(
        dest_render_frame_host);
  }

  return dest_render_frame_host;
}
