void ResourceDispatcherHostImpl::RegisterResourceMessageDelegate(
    const GlobalRequestID& id, ResourceMessageDelegate* delegate) {
  DelegateMap::iterator it = delegate_map_.find(id);
  if (it == delegate_map_.end()) {
    it = delegate_map_.insert(
                           std::make_pair(
                               id,
                               new base::ObserverList<ResourceMessageDelegate>))
             .first;
  }
  it->second->AddObserver(delegate);
}
