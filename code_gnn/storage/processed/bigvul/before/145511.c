void ResourceDispatcherHostImpl::OnDidChangePriority(
    int request_id,
    net::RequestPriority new_priority,
    int intra_priority_value) {
  ResourceLoader* loader = GetLoader(filter_->child_id(), request_id);
  if (!loader)
    return;

  scheduler_->ReprioritizeRequest(loader->request(), new_priority,
                                  intra_priority_value);
}
