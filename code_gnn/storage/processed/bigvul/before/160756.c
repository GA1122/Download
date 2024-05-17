RenderFrameImpl::~RenderFrameImpl() {
  while (!file_chooser_completions_.empty()) {
    if (file_chooser_completions_.front()->completion) {
      file_chooser_completions_.front()->completion->DidChooseFile(
          WebVector<WebString>());
    }
    file_chooser_completions_.pop_front();
  }

  for (auto& observer : observers_)
    observer.RenderFrameGone();
  for (auto& observer : observers_)
    observer.OnDestruct();

  base::trace_event::TraceLog::GetInstance()->RemoveProcessLabel(routing_id_);

  RenderThreadImpl* render_thread = RenderThreadImpl::current();
  InputHandlerManager* input_handler_manager =
      render_thread ? render_thread->input_handler_manager() : nullptr;
  if (input_handler_manager)
    input_handler_manager->UnregisterRoutingID(GetRoutingID());

  if (auto* factory = AudioOutputIPCFactory::get())
    factory->MaybeDeregisterRemoteFactory(GetRoutingID());

  if (is_main_frame_) {
    render_view_->main_render_frame_ = nullptr;
  }

  g_routing_id_frame_map.Get().erase(routing_id_);
  RenderThread::Get()->RemoveRoute(routing_id_);
}
