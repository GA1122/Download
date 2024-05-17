void ChromeContentBrowserClient::SiteInstanceDeleting(
    SiteInstance* site_instance) {
  if (!site_instance->HasProcess())
    return;

  Profile* profile = Profile::FromBrowserContext(
      site_instance->GetBrowserContext());
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!service)
    return;

  const Extension* extension =
      service->extensions()->GetExtensionOrAppByURL(
          ExtensionURLInfo(site_instance->GetSiteURL()));
  if (!extension)
    return;

  service->process_map()->Remove(extension->id(),
                                 site_instance->GetProcess()->GetID(),
                                 site_instance->GetId());
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&ExtensionInfoMap::UnregisterExtensionProcess,
                 extensions::ExtensionSystem::Get(profile)->info_map(),
                 extension->id(),
                 site_instance->GetProcess()->GetID(),
                 site_instance->GetId()));
 }
