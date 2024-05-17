void ResourceDispatcherHostImpl::OnRenderViewHostCreated(
    int child_id,
    int route_id,
    net::URLRequestContextGetter* url_request_context_getter) {
  auto* host = ResourceDispatcherHostImpl::Get();
  if (host && host->scheduler_) {
    host->scheduler_->OnClientCreated(
        child_id, route_id,
        url_request_context_getter->GetURLRequestContext()
            ->network_quality_estimator());
  }
}
