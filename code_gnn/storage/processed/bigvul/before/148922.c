RenderFrameHostImpl* RenderFrameHostManager::UpdateStateForNavigate(
    const GURL& dest_url,
    SiteInstance* source_instance,
    SiteInstance* dest_instance,
    ui::PageTransition transition,
    bool dest_is_restore,
    bool dest_is_view_source_mode,
    const GlobalRequestID& transferred_request_id,
    int bindings,
    bool is_reload) {
  SiteInstance* current_instance = render_frame_host_->GetSiteInstance();
  bool was_server_redirect = transfer_navigation_handle_ &&
                             transfer_navigation_handle_->WasServerRedirect();
  scoped_refptr<SiteInstance> new_instance = GetSiteInstanceForNavigation(
      dest_url, source_instance, dest_instance, nullptr, transition,
      dest_is_restore, dest_is_view_source_mode, was_server_redirect);

  bool allowed_to_swap_process =
      frame_tree_node_->IsMainFrame() ||
      CanSubframeSwapProcess(dest_url, source_instance, dest_instance,
                             was_server_redirect);

  if (transfer_navigation_handle_.get() &&
      transfer_navigation_handle_->GetGlobalRequestID() ==
          transferred_request_id) {
    RenderFrameHostImpl* transferring_rfh =
        transfer_navigation_handle_->GetRenderFrameHost();
    bool transfer_started_from_current_rfh =
        transferring_rfh == render_frame_host_.get();
    bool should_transfer =
        new_instance.get() != transferring_rfh->GetSiteInstance() &&
        (!transfer_started_from_current_rfh || allowed_to_swap_process);
    if (should_transfer)
      transfer_navigation_handle_->Transfer();
  }

  if (pending_render_frame_host_) {
    if (pending_render_frame_host_->GetSiteInstance() != new_instance) {
      CancelPending();
    } else {
      CHECK(pending_render_frame_host_->IsRenderFrameLive());
    }
  }

  if (new_instance.get() != current_instance && allowed_to_swap_process) {
    TRACE_EVENT_INSTANT2(
        "navigation",
        "RenderFrameHostManager::UpdateStateForNavigate:New SiteInstance",
        TRACE_EVENT_SCOPE_THREAD,
        "current_instance id", current_instance->GetId(),
        "new_instance id", new_instance->GetId());


    if (!pending_render_frame_host_)
      CreatePendingRenderFrameHost(current_instance, new_instance.get());
    DCHECK(pending_render_frame_host_);
    if (!pending_render_frame_host_)
      return nullptr;
    DCHECK_EQ(new_instance, pending_render_frame_host_->GetSiteInstance());

    pending_render_frame_host_->UpdatePendingWebUI(dest_url, bindings);
    pending_render_frame_host_->CommitPendingWebUI();
    DCHECK_EQ(GetNavigatingWebUI(), pending_render_frame_host_->web_ui());

    if (pending_render_frame_host_->web_ui()) {
      pending_render_frame_host_->web_ui()->RenderFrameCreated(
          pending_render_frame_host_.get());
    }

    if (!render_frame_host_->IsRenderFrameLive()) {
      if (GetRenderFrameProxyHost(new_instance.get())) {
        pending_render_frame_host_->Send(
            new FrameMsg_SwapIn(pending_render_frame_host_->GetRoutingID()));
      }
      CommitPending();
      return render_frame_host_.get();
    }

    bool is_transfer = transferred_request_id != GlobalRequestID();
    if (is_transfer) {
      DCHECK(transfer_navigation_handle_ &&
             transfer_navigation_handle_->GetGlobalRequestID() ==
                 transferred_request_id);
    } else if (!pending_render_frame_host_->are_navigations_suspended()) {
      render_frame_host_->Send(new FrameMsg_Stop(
          render_frame_host_->GetRoutingID()));
      pending_render_frame_host_->SetNavigationsSuspended(true,
                                                          base::TimeTicks());
      render_frame_host_->DispatchBeforeUnload(true, is_reload);
    }

    return pending_render_frame_host_.get();
  }


  DeleteRenderFrameProxyHost(new_instance.get());

  UpdatePendingWebUIOnCurrentFrameHost(dest_url, bindings);

  if (dest_is_view_source_mode) {
    DCHECK(!render_frame_host_->GetParent());
    render_frame_host_->Send(
        new FrameMsg_EnableViewSourceMode(render_frame_host_->GetRoutingID()));
  }

  return render_frame_host_.get();
}
