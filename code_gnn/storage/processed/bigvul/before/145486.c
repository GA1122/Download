scoped_ptr<ResourceHandler> ResourceDispatcherHostImpl::CreateResourceHandler(
    net::URLRequest* request,
    const ResourceHostMsg_Request& request_data,
    IPC::Message* sync_result,
    int route_id,
    int process_type,
    int child_id,
    ResourceContext* resource_context) {
  tracked_objects::ScopedTracker tracking_profile(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "456331 ResourceDispatcherHostImpl::CreateResourceHandler"));
  scoped_ptr<ResourceHandler> handler;
  if (sync_result) {
    if (request_data.download_to_file) {
      bad_message::ReceivedBadMessage(filter_, bad_message::RDH_BAD_DOWNLOAD);
      return scoped_ptr<ResourceHandler>();
    }

    handler.reset(new SyncResourceHandler(request, sync_result, this));
  } else {
    handler.reset(new AsyncResourceHandler(request, this));

    if (request_data.download_to_file) {
      handler.reset(
          new RedirectToFileResourceHandler(std::move(handler), request));
    }
  }

  if (!sync_result && IsDetachableResourceType(request_data.resource_type)) {
    handler.reset(new DetachableResourceHandler(
        request,
        base::TimeDelta::FromMilliseconds(kDefaultDetachableCancelDelayMs),
        std::move(handler)));
  }

  if (!IsBrowserSideNavigationEnabled()) {
    bool is_swappable_navigation =
        request_data.resource_type == RESOURCE_TYPE_MAIN_FRAME;
    if (!is_swappable_navigation &&
        SiteIsolationPolicy::AreCrossProcessFramesPossible()) {
      is_swappable_navigation =
          request_data.resource_type == RESOURCE_TYPE_SUB_FRAME;
    }
    if (is_swappable_navigation && process_type == PROCESS_TYPE_RENDERER)
      handler.reset(new CrossSiteResourceHandler(std::move(handler), request));
  }

  return AddStandardHandlers(request, request_data.resource_type,
                             resource_context, filter_->appcache_service(),
                             child_id, route_id, std::move(handler));
}
