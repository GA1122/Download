void ChromeContentBrowserClient::GetStoragePartitionConfigForSite(
    content::BrowserContext* browser_context,
    const GURL& site,
    bool can_be_default,
    std::string* partition_domain,
    std::string* partition_name,
    bool* in_memory) {
  partition_domain->clear();
  partition_name->clear();
  *in_memory = false;

  if (site.SchemeIs(chrome::kGuestScheme)) {
    CHECK(site.has_host());
    *partition_domain = site.host();
    *in_memory = (site.path() != "/persist");
    *partition_name = net::UnescapeURLComponent(site.query(),
                                                net::UnescapeRule::NORMAL);
  } else if (site.SchemeIs(extensions::kExtensionScheme)) {
    bool is_isolated = !can_be_default;
    if (can_be_default) {
      const Extension* extension = NULL;
      Profile* profile = Profile::FromBrowserContext(browser_context);
      ExtensionService* extension_service =
          extensions::ExtensionSystem::Get(profile)->extension_service();
      if (extension_service) {
        extension = extension_service->extensions()->
            GetExtensionOrAppByURL(ExtensionURLInfo(site));
        if (extension && extension->is_storage_isolated()) {
          is_isolated = true;
        }
      }
    }

    if (is_isolated) {
      CHECK(site.has_host());
      *partition_domain = site.host();
      *in_memory = false;
      partition_name->clear();
    }
  }

  CHECK(can_be_default || !partition_domain->empty());
}
