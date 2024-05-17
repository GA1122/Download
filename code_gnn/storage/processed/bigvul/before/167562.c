void RenderFrameHostManager::CommitPending() {
  TRACE_EVENT1("navigation", "RenderFrameHostManager::CommitPending",
               "FrameTreeNode id", frame_tree_node_->frame_tree_node_id());
  DCHECK(speculative_render_frame_host_);

#if defined(OS_MACOSX)
  gfx::ScopedCocoaDisableScreenUpdates disabler;
#endif   

  bool is_main_frame = frame_tree_node_->IsMainFrame();

  bool will_focus_location_bar =
      is_main_frame && delegate_->FocusLocationBarByDefault();

  bool focus_render_view = !will_focus_location_bar &&
                           render_frame_host_->GetView() &&
                           render_frame_host_->GetView()->HasFocus();

  frame_tree_node_->ResetForNewProcess();

  std::unique_ptr<RenderFrameHostImpl> old_render_frame_host;
  DCHECK(speculative_render_frame_host_);
  old_render_frame_host =
      SetRenderFrameHost(std::move(speculative_render_frame_host_));

  if (is_main_frame &&
      old_render_frame_host->render_view_host()->GetWidget()->GetView()) {
    old_render_frame_host->render_view_host()->GetWidget()->GetView()->Hide();
  }

  delegate_->UpdateRenderViewSizeForRenderManager(is_main_frame);

  if (will_focus_location_bar) {
    delegate_->SetFocusToLocationBar(false);
  } else if (focus_render_view && render_frame_host_->GetView()) {
    if (is_main_frame) {
      render_frame_host_->GetView()->Focus();
    } else {
      FrameTreeNode* focused_frame =
          frame_tree_node_->frame_tree()->GetFocusedFrame();
      if (focused_frame && !focused_frame->IsMainFrame() &&
          focused_frame->current_frame_host()->GetSiteInstance() !=
              render_frame_host_->GetSiteInstance()) {
        focused_frame->render_manager()
            ->GetRenderFrameProxyHost(render_frame_host_->GetSiteInstance())
            ->SetFocusedFrame();
      }
      frame_tree_node_->frame_tree()->SetPageFocus(
          render_frame_host_->GetSiteInstance(), true);
    }
  }

  delegate_->NotifySwappedFromRenderManager(
      old_render_frame_host.get(), render_frame_host_.get(), is_main_frame);

  if (is_main_frame && old_render_frame_host->GetView() &&
      render_frame_host_->GetView()) {
    render_frame_host_->GetView()->TakeFallbackContentFrom(
        old_render_frame_host->GetView());
  }

  if (is_main_frame) {
    RenderViewHostImpl* rvh = render_frame_host_->render_view_host();
    rvh->set_main_frame_routing_id(render_frame_host_->routing_id());

    if (!rvh->is_active())
      rvh->PostRenderViewReady();

    rvh->SetIsActive(true);
    rvh->set_is_swapped_out(false);
    old_render_frame_host->render_view_host()->set_main_frame_routing_id(
        MSG_ROUTING_NONE);
  }

  base::Optional<gfx::Size> old_size = old_render_frame_host->frame_size();

  SwapOutOldFrame(std::move(old_render_frame_host));

  DeleteRenderFrameProxyHost(render_frame_host_->GetSiteInstance());

  RenderFrameProxyHost* proxy_to_parent = GetProxyToParent();
  if (proxy_to_parent) {
    proxy_to_parent->SetChildRWHView(render_frame_host_->GetView(),
                                     old_size ? &*old_size : nullptr);
  }

  bool new_rfh_has_view = !!render_frame_host_->GetView();
  if (!delegate_->IsHidden() && new_rfh_has_view) {
    if (!is_main_frame &&
        !render_frame_host_->render_view_host()->is_active()) {
      RenderFrameProxyHost* proxy =
          frame_tree_node_->frame_tree()
              ->root()
              ->render_manager()
              ->GetRenderFrameProxyHost(render_frame_host_->GetSiteInstance());
      proxy->Send(new PageMsg_WasShown(proxy->GetRoutingID()));
    }

    render_frame_host_->GetView()->Show();
  }
  render_frame_host_->GetProcess()->RemovePendingView();

  if (!new_rfh_has_view) {
    DCHECK(!render_frame_host_->IsRenderFrameLive());
    DCHECK(!render_frame_host_->render_view_host()->IsRenderViewLive());
    render_frame_host_->ResetLoadingState();
    delegate_->RenderProcessGoneFromRenderManager(
        render_frame_host_->render_view_host());
  }

  CHECK(!GetRenderFrameProxyHost(render_frame_host_->GetSiteInstance()));
}
