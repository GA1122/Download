void ResourceDispatcherHostImpl::MarkAsTransferredNavigation(
    const GlobalRequestID& id) {
  GetLoader(id)->MarkAsTransferring();
}
