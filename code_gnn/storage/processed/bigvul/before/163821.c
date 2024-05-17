    ShouldSwapBrowsingInstancesForNavigation(SiteInstance* site_instance,
                                             const GURL& current_url,
                                             const GURL& new_url) {
  ExtensionRegistry* registry =
      ExtensionRegistry::Get(site_instance->GetBrowserContext());
  if (!registry)
    return false;

  const Extension* current_extension =
      registry->enabled_extensions().GetExtensionOrAppByURL(current_url);
  bool is_current_url_for_web_store =
      current_extension && current_extension->id() == kWebStoreAppId;

  const Extension* new_extension =
      registry->enabled_extensions().GetExtensionOrAppByURL(new_url);
  bool is_new_url_for_web_store =
      new_extension && new_extension->id() == kWebStoreAppId;

  ProcessMap* process_map = ProcessMap::Get(site_instance->GetBrowserContext());
  if (is_new_url_for_web_store && site_instance->HasProcess() &&
      !process_map->Contains(new_extension->id(),
                             site_instance->GetProcess()->GetID()))
    return true;

  return is_current_url_for_web_store != is_new_url_for_web_store;
}
