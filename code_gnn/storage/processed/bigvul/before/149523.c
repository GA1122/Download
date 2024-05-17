bool LoadingDataCollector::ShouldRecordResourceLoad(
    const NavigationID& navigation_id,
    const content::mojom::ResourceLoadInfo& resource_load_info) const {
  const GURL& url = resource_load_info.url;
  if (!url.is_valid() || !url.SchemeIsHTTPOrHTTPS())
    return false;

  if (!g_allow_port_in_urls && url.has_port())
    return false;

  if (base::FeatureList::IsEnabled(
          features::kLoadingOnlyLearnHighPriorityResources) &&
      resource_load_info.request_priority < net::MEDIUM) {
    return false;
  }

  if (!IsHandledResourceType(resource_load_info.resource_type,
                             resource_load_info.mime_type)) {
    return false;
  }
  if (resource_load_info.method != "GET")
    return false;

  return true;
}
