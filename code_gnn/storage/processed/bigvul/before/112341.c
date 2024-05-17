bool ResourceDispatcherHostImpl::IsTransferredNavigation(
    const GlobalRequestID& id) const {
  ResourceLoader* loader = GetLoader(id);
  return loader ? loader->is_transferring() : false;
}
