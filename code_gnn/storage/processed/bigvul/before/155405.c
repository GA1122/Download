bool ChromeContentBrowserClient::IsSafeRedirectTarget(
    const GURL& url,
    content::ResourceContext* context) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (url.SchemeIs(extensions::kExtensionScheme)) {
    ProfileIOData* io_data = ProfileIOData::FromResourceContext(context);
    const Extension* extension =
        io_data->GetExtensionInfoMap()->extensions().GetByID(url.host());
    if (!extension)
      return false;
    return extensions::WebAccessibleResourcesInfo::IsResourceWebAccessible(
        extension, url.path());
  }
#endif   
  return true;
}
