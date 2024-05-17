bool BrowserPpapiHostImpl::IsThrottled(PP_Instance instance) const {
  auto it = instance_map_.find(instance);
  if (it != instance_map_.end())
    return it->second->is_throttled;

  return false;
}
