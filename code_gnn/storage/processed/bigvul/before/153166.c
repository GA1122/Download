void Compositor::RemoveChildFrameSink(const viz::FrameSinkId& frame_sink_id) {
  if (!context_factory_private_)
    return;
  auto it = child_frame_sinks_.find(frame_sink_id);
  DCHECK(it != child_frame_sinks_.end());
  DCHECK(it->is_valid());
  context_factory_private_->GetHostFrameSinkManager()
      ->UnregisterFrameSinkHierarchy(frame_sink_id_, *it);
  child_frame_sinks_.erase(it);
}
