void ResourceDispatcherHostImpl::ReprioritizeRequest(
    net::URLRequest* request,
    net::RequestPriority priority) {
  scheduler_->ReprioritizeRequest(request, priority);
}
