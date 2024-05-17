bool Browser::MaybeCreateBackgroundContents(
    content::SiteInstance* source_site_instance,
    content::RenderFrameHost* opener,
    const GURL& opener_url,
    int32_t route_id,
    int32_t main_frame_route_id,
    int32_t main_frame_widget_route_id,
    const std::string& frame_name,
    const GURL& target_url,
    const std::string& partition_id,
    content::SessionStorageNamespace* session_storage_namespace) {
  extensions::ExtensionService* extensions_service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();

  if (!opener_url.is_valid() || frame_name.empty() || !extensions_service ||
      !extensions_service->is_ready())
    return false;

  const Extension* extension = extensions::ExtensionRegistry::Get(profile_)
                                   ->enabled_extensions()
                                   .GetHostedAppByURL(opener_url);
  if (!extension)
    return false;

  BackgroundContentsService* service =
      BackgroundContentsServiceFactory::GetForProfile(profile_);
  if (!service)
    return false;

  extensions::ProcessMap* process_map = extensions::ProcessMap::Get(profile_);
  if (!source_site_instance->GetProcess() ||
      !process_map->Contains(extension->id(),
                             source_site_instance->GetProcess()->GetID())) {
    return false;
  }

  bool allow_js_access = extensions::BackgroundInfo::AllowJSAccess(extension);
  BackgroundContents* existing =
      service->GetAppBackgroundContents(extension->id());
  if (existing) {
    if (!allow_js_access)
      return true;
    delete existing;
  }

  BackgroundContents* contents = nullptr;
  if (allow_js_access) {
    contents = service->CreateBackgroundContents(
        source_site_instance, opener, route_id, main_frame_route_id,
        main_frame_widget_route_id, profile_, frame_name, extension->id(),
        partition_id, session_storage_namespace);
  } else {
    contents = service->CreateBackgroundContents(
        content::SiteInstance::Create(
            source_site_instance->GetBrowserContext()),
        nullptr, MSG_ROUTING_NONE, MSG_ROUTING_NONE, MSG_ROUTING_NONE, profile_,
        frame_name, extension->id(), partition_id, session_storage_namespace);

    if (contents) {
      contents->web_contents()->GetController().LoadURL(
          target_url, content::Referrer(), ui::PAGE_TRANSITION_LINK,
          std::string());   
    }
  }

  return contents != NULL;
}
