void ResourceDispatcherHostImpl::UnregisterResourceMessageDelegate(
    const GlobalRequestID& id, ResourceMessageDelegate* delegate) {
  DCHECK(ContainsKey(delegate_map_, id));
  DelegateMap::iterator it = delegate_map_.find(id);
  DCHECK(it->second->HasObserver(delegate));
  it->second->RemoveObserver(delegate);
  if (!it->second->might_have_observers()) {
    delete it->second;
    delegate_map_.erase(it);
  }
}
