RenderFrameImpl::~RenderFrameImpl() {
  for (auto& observer : observers_)
    observer.RenderFrameGone();
  for (auto& observer : observers_)
    observer.OnDestruct();

  base::trace_event::TraceLog::GetInstance()->RemoveProcessLabel(routing_id_);

  if (auto* factory = AudioOutputIPCFactory::get())
    factory->MaybeDeregisterRemoteFactory(GetRoutingID());

  if (auto* thread = RenderThreadImpl::current()) {
    if (auto* controller = thread->low_memory_mode_controller())
      controller->OnFrameDestroyed(IsMainFrame());
  }

  if (is_main_frame_) {
    render_view_->main_render_frame_ = nullptr;
  }

  g_routing_id_frame_map.Get().erase(routing_id_);
  RenderThread::Get()->RemoveRoute(routing_id_);
}
