 RenderFrameImpl::~RenderFrameImpl() {
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, RenderFrameGone());
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, OnDestruct());

  base::trace_event::TraceLog::GetInstance()->RemoveProcessLabel(routing_id_);

#if defined(VIDEO_HOLE)
  if (contains_media_player_)
    render_view_->UnregisterVideoHoleFrame(this);
#endif

  if (!is_subframe_) {
    if (render_frame_proxy_ &&
        !SiteIsolationPolicy::IsSwappedOutStateForbidden()) {
      render_frame_proxy_->frameDetached(
          blink::WebRemoteFrameClient::DetachType::Remove);
    }

    render_view_->main_render_frame_ = nullptr;
  }

  render_view_->UnregisterRenderFrame(this);
  g_routing_id_frame_map.Get().erase(routing_id_);
  RenderThread::Get()->RemoveRoute(routing_id_);
}
