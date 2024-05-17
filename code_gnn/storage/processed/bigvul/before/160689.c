void RenderFrameImpl::OnSwapOut(
    int proxy_routing_id,
    bool is_loading,
    const FrameReplicationState& replicated_frame_state) {
  TRACE_EVENT1("navigation,rail", "RenderFrameImpl::OnSwapOut",
               "id", routing_id_);
  RenderFrameProxy* proxy = nullptr;


  SendUpdateState();

  CHECK_NE(proxy_routing_id, MSG_ROUTING_NONE);
  proxy = RenderFrameProxy::CreateProxyToReplaceFrame(
      this, proxy_routing_id, replicated_frame_state.scope);

  if (is_main_frame_)
    frame_->DispatchUnloadEvent();

  if (is_main_frame_)
    render_view_->SetSwappedOut(true);

  RenderViewImpl* render_view = render_view_;
  bool is_main_frame = is_main_frame_;
  int routing_id = GetRoutingID();

  bool success = frame_->Swap(proxy->web_frame());

  if (is_main_frame)
    CHECK(!render_view->main_render_frame_);

  if (!success) {
    proxy->FrameDetached(blink::WebRemoteFrameClient::DetachType::kSwap);
    return;
  }

  if (is_loading)
    proxy->OnDidStartLoading();

  proxy->SetReplicatedState(replicated_frame_state);

  if (is_main_frame)
    render_view->WasSwappedOut();

  RenderThread::Get()->Send(new FrameHostMsg_SwapOut_ACK(routing_id));
}
