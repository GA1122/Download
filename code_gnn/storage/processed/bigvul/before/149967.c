bool LayerTreeHostImpl::IsUIResourceOpaque(UIResourceId uid) const {
  UIResourceMap::const_iterator iter = ui_resource_map_.find(uid);
  DCHECK(iter != ui_resource_map_.end());
  return iter->second.opaque;
}
