content::HeaderInterceptorResult CheckOriginHeader(
    content::ResourceContext* resource_context,
    int child_id,
    const GURL& origin) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (!origin.SchemeIs(kExtensionScheme))
    return content::HeaderInterceptorResult::CONTINUE;

  ProfileIOData* io_data = ProfileIOData::FromResourceContext(resource_context);
  InfoMap* extension_info_map = io_data->GetExtensionInfoMap();
  const Extension* extension =
      extension_info_map->extensions().GetExtensionOrAppByURL(origin);
  if (!extension)
    return content::HeaderInterceptorResult::FAIL;

  const ProcessMap& process_map = extension_info_map->process_map();
  if (extension->is_platform_app() &&
      !process_map.Contains(extension->id(), child_id)) {
    if (!extension->permissions_data()->HasAPIPermission(
            extensions::APIPermission::kWebView)) {
      return content::HeaderInterceptorResult::KILL;
    }

    std::string owner_extension_id;
    int owner_process_id;
    WebViewRendererState::GetInstance()->GetOwnerInfo(
        child_id, &owner_process_id, &owner_extension_id);
    const Extension* owner_extension =
        extension_info_map->extensions().GetByID(owner_extension_id);
    if (!owner_extension || owner_extension != extension)
      return content::HeaderInterceptorResult::KILL;

    return content::HeaderInterceptorResult::CONTINUE;
  }

  return content::HeaderInterceptorResult::CONTINUE;
}
