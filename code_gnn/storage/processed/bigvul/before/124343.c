static content::RenderProcessHost* GetExtensionProcess(
    Profile* profile, const std::string& extension_id) {
  SiteInstance* site_instance =
      extensions::ExtensionSystem::Get(profile)->process_manager()->
          GetSiteInstanceForURL(
              Extension::GetBaseURLFromExtensionId(extension_id));

  if (!site_instance->HasProcess())
    return NULL;

  return site_instance->GetProcess();
}
