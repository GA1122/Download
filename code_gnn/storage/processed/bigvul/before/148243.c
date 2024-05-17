bool BrowserPpapiHostImpl::IsValidInstance(PP_Instance instance) const {
  return instance_map_.find(instance) != instance_map_.end();
}
