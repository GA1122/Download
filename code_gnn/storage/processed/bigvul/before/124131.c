bool ChromeContentBrowserClient::ShouldSwapProcessesForRedirect(
    content::ResourceContext* resource_context, const GURL& current_url,
    const GURL& new_url) {
  ProfileIOData* io_data = ProfileIOData::FromResourceContext(resource_context);
  return extensions::CrossesExtensionProcessBoundary(
      io_data->GetExtensionInfoMap()->extensions(),
      ExtensionURLInfo(current_url), ExtensionURLInfo(new_url), false);
}
