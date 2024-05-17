void RenderFrameHostImpl::DispatchBeforeUnload(bool for_navigation,
                                               bool is_reload) {
  DCHECK(for_navigation || !is_reload);

  if (IsBrowserSideNavigationEnabled() && !for_navigation) {
    frame_tree_node_->ResetNavigationRequest(false, true);
  }

  if (!ShouldDispatchBeforeUnload()) {
    DCHECK(!(IsBrowserSideNavigationEnabled() && for_navigation));
    frame_tree_node_->render_manager()->OnBeforeUnloadACK(
        for_navigation, true, base::TimeTicks::Now());
    return;
  }
  TRACE_EVENT_ASYNC_BEGIN1("navigation", "RenderFrameHostImpl BeforeUnload",
                           this, "&RenderFrameHostImpl", (void*)this);

  if (is_waiting_for_beforeunload_ack_) {
    unload_ack_is_for_navigation_ =
        unload_ack_is_for_navigation_ && for_navigation;
  } else {
    is_waiting_for_beforeunload_ack_ = true;
    unload_ack_is_for_navigation_ = for_navigation;
    if (render_view_host_->GetDelegate()->IsJavaScriptDialogShowing()) {
      SimulateBeforeUnloadAck();
    } else {
      if (beforeunload_timeout_) {
        beforeunload_timeout_->Start(
            TimeDelta::FromMilliseconds(RenderViewHostImpl::kUnloadTimeoutMS));
      }
      send_before_unload_start_time_ = base::TimeTicks::Now();
      Send(new FrameMsg_BeforeUnload(routing_id_, is_reload));
    }
  }
}
