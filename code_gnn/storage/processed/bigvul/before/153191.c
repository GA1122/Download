Compositor::~Compositor() {
  TRACE_EVENT0("shutdown", "Compositor::destructor");

  for (auto& observer : observer_list_)
    observer.OnCompositingShuttingDown(this);

  for (auto& observer : animation_observer_list_)
    observer.OnCompositingShuttingDown(this);

  if (root_layer_)
    root_layer_->ResetCompositor();

  if (animation_timeline_)
    animation_host_->RemoveAnimationTimeline(animation_timeline_.get());

  host_.reset();

  context_factory_->RemoveCompositor(this);
  if (context_factory_private_) {
    auto* host_frame_sink_manager =
        context_factory_private_->GetHostFrameSinkManager();
    for (auto& client : child_frame_sinks_) {
      DCHECK(client.is_valid());
      host_frame_sink_manager->UnregisterFrameSinkHierarchy(frame_sink_id_,
                                                            client);
    }
    host_frame_sink_manager->InvalidateFrameSinkId(frame_sink_id_);
  }
}
