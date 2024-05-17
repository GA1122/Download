ResourceId LayerTreeHostImpl::ResourceIdForUIResource(UIResourceId uid) const {
  UIResourceMap::const_iterator iter = ui_resource_map_.find(uid);
  if (iter != ui_resource_map_.end())
    return iter->second.resource_id;
  return 0;
}
