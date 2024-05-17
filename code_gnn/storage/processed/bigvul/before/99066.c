void ResourceDispatcherHost::ApplyExtensionMessageFilterPolicy(
    const GURL& url,
    const ResourceType::Type& resource_type,
    ResourceDispatcherHostRequestInfo* request_info) {
  if (url.SchemeIs(chrome::kExtensionScheme) &&
      request_info->filter_policy() == FilterPolicy::DONT_FILTER &&
      resource_type == ResourceType::STYLESHEET) {
    request_info->set_filter_policy(FilterPolicy::FILTER_EXTENSION_MESSAGES);
  }
}
