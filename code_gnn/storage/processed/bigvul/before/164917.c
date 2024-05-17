void ResourceDispatcherHostImpl::CancelRequestFromRenderer(
    GlobalRequestID request_id) {
  ResourceLoader* loader = GetLoader(request_id);

  if (!loader)
    return;

  loader->CancelRequest(true);
}
