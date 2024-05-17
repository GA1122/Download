AppCacheRequestHandler* AppCacheHost::CreateRequestHandler(
    net::URLRequest* request,
    ResourceType resource_type,
    bool should_reset_appcache) {
  if (is_for_dedicated_worker()) {
    AppCacheHost* parent_host = GetParentAppCacheHost();
    if (parent_host)
      return parent_host->CreateRequestHandler(
          request, resource_type, should_reset_appcache);
    return NULL;
  }

  if (AppCacheRequestHandler::IsMainResourceType(resource_type)) {
    first_party_url_ = request->first_party_for_cookies();
    return new AppCacheRequestHandler(
        this, resource_type, should_reset_appcache);
  }

  if ((associated_cache() && associated_cache()->is_complete()) ||
      is_selection_pending()) {
    return new AppCacheRequestHandler(
        this, resource_type, should_reset_appcache);
  }
  return NULL;
}
