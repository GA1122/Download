bool ChromeContentBrowserClient::ShouldTryToUseExistingProcessHost(
    content::BrowserContext* browser_context, const GURL& url) {
  if (!url.is_valid())
    return false;

  Profile* profile = Profile::FromBrowserContext(browser_context);
  ExtensionService* service = !profile ? NULL :
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!service)
    return false;

  const Extension* extension =
      service->extensions()->GetExtensionOrAppByURL(ExtensionURLInfo(url));
  if (!extension)
    return false;
  if (!extension->has_background_page())
    return false;

  std::set<int> process_ids;
  size_t max_process_count =
      content::RenderProcessHost::GetMaxRendererProcessCount();

  std::vector<Profile*> profiles = g_browser_process->profile_manager()->
      GetLoadedProfiles();
  for (size_t i = 0; i < profiles.size(); ++i) {
    ExtensionProcessManager* epm =
        extensions::ExtensionSystem::Get(profiles[i])->process_manager();
    for (ExtensionProcessManager::const_iterator iter =
             epm->background_hosts().begin();
         iter != epm->background_hosts().end(); ++iter) {
      const extensions::ExtensionHost* host = *iter;
      process_ids.insert(host->render_process_host()->GetID());
    }
  }

  if (process_ids.size() >
      (max_process_count * chrome::kMaxShareOfExtensionProcesses)) {
    return true;
  }

  return false;
}
