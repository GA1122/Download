BrowserPpapiHostImpl::~BrowserPpapiHostImpl() {
  message_filter_->OnHostDestroyed();

  for (auto& instance_data : instance_map_) {
    for (auto& observer : instance_data.second->observer_list)
      observer.OnHostDestroyed();
  }

  ppapi_host_.reset();
}
