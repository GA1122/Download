bool ChromeContentBrowserClient::IsSuitableHost(
    content::RenderProcessHost* process_host,
    const GURL& site_url) {
  Profile* profile =
      Profile::FromBrowserContext(process_host->GetBrowserContext());
  if (!profile)
    return true;

  InstantService* instant_service =
      InstantServiceFactory::GetForProfile(profile);
  if (instant_service &&
      instant_service->IsInstantProcess(process_host->GetID()))
    return chrome::search::ShouldAssignURLToInstantRenderer(site_url, profile);

  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  extensions::ProcessMap* process_map = service->process_map();

  if (site_url.GetOrigin() == GURL(chrome::kChromeUITaskManagerURL).GetOrigin())
    return false;

  if (!service || !process_map)
    return true;

  RenderProcessHostPrivilege privilege_required =
      GetPrivilegeRequiredByUrl(site_url, service);
  return GetProcessPrivilege(process_host, process_map, service) ==
      privilege_required;
}
