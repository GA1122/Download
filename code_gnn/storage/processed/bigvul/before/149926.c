void LayerTreeHostImpl::DeleteUIResource(UIResourceId uid) {
  ResourceId id = ResourceIdForUIResource(uid);
  if (id) {
    if (has_valid_compositor_frame_sink_)
      resource_provider_->DeleteResource(id);
    ui_resource_map_.erase(uid);
  }
  MarkUIResourceNotEvicted(uid);
}
