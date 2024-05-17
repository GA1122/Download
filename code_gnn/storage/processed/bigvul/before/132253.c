void RenderFrameImpl::OnSwapOut(
    int proxy_routing_id,
    bool is_loading,
    const FrameReplicationState& replicated_frame_state) {
  TRACE_EVENT1("navigation", "RenderFrameImpl::OnSwapOut", "id", routing_id_);
  RenderFrameProxy* proxy = NULL;
  bool swapped_out_forbidden =
      SiteIsolationPolicy::IsSwappedOutStateForbidden();
  bool is_main_frame = !frame_->parent();

  CHECK_IMPLIES(!is_main_frame,
                SiteIsolationPolicy::AreCrossProcessFramesPossible());

  if (!is_swapped_out_) {

    render_view_->SendUpdateState();

    if (proxy_routing_id != MSG_ROUTING_NONE) {
      proxy = RenderFrameProxy::CreateProxyToReplaceFrame(
          this, proxy_routing_id, replicated_frame_state.scope);
    }

    if (is_main_frame)
      frame_->dispatchUnloadEvent();

    if (is_main_frame)
      render_view_->SetSwappedOut(true);
    is_swapped_out_ = true;

    if (proxy)
      set_render_frame_proxy(proxy);

    if (!swapped_out_forbidden)
      OnStop();

    if (!is_main_frame && proxy)
      proxy->web_frame()->initializeFromFrame(frame_);

    if (!swapped_out_forbidden)
      NavigateToSwappedOutURL();

    if (is_main_frame) {
      render_view_->webview()->setVisibilityState(
          blink::WebPageVisibilityStateHidden, false);
    }
  }

  if (is_main_frame)
    render_view_->suppress_dialogs_until_swap_out_ = false;

  Send(new FrameHostMsg_SwapOut_ACK(routing_id_));

  RenderViewImpl* render_view = render_view_.get();

  if (proxy && swapped_out_forbidden) {
    frame_->swap(proxy->web_frame());

    if (is_loading)
      proxy->OnDidStartLoading();
  }

  if (proxy && swapped_out_forbidden)
    proxy->SetReplicatedState(replicated_frame_state);

  if (is_main_frame) {
    render_view->WasSwappedOut();
  }
}
