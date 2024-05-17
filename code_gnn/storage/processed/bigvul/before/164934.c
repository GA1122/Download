net::URLRequest* ResourceDispatcherHostImpl::GetURLRequest(
    const GlobalRequestID& id) {
  ResourceLoader* loader = GetLoader(id);
  if (!loader)
    return nullptr;

  return loader->request();
}
