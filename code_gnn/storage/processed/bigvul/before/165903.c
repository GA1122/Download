void RenderFrameImpl::FrameDetached(DetachType type) {
  for (auto& observer : observers_)
    observer.FrameDetached();

  SendUpdateState();

  if (type == DetachType::kRemove)
    Send(new FrameHostMsg_Detach(routing_id_));

  GetRenderWidget()->UnregisterRenderFrame(this);
  if (is_main_frame_) {
    render_view_->DetachWebFrameWidget();
    render_widget_->AbortWarmupCompositor();
  } else if (render_widget_) {
    render_widget_->CloseForFrame();
  }

  auto it = g_frame_map.Get().find(frame_);
  CHECK(it != g_frame_map.Get().end());
  CHECK_EQ(it->second, this);
  g_frame_map.Get().erase(it);

  frame_->Close();
  frame_ = nullptr;

  if (proxy_routing_id_ != MSG_ROUTING_NONE) {
    RenderFrameProxy* proxy =
        RenderFrameProxy::FromRoutingID(proxy_routing_id_);

    CHECK(proxy);
    CHECK_EQ(routing_id_, proxy->provisional_frame_routing_id());

    proxy->set_provisional_frame_routing_id(MSG_ROUTING_NONE);
  }

  delete this;
}
