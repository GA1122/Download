void LayerTreeHostImpl::ClearUIResources() {
  for (UIResourceMap::const_iterator iter = ui_resource_map_.begin();
       iter != ui_resource_map_.end(); ++iter) {
    evicted_ui_resources_.insert(iter->first);
    resource_provider_->DeleteResource(iter->second.resource_id);
  }
  ui_resource_map_.clear();
}
