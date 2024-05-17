  const Extension* GetInstalledApp(WebContents* contents) {
    content::BrowserContext* browser_context = contents->GetBrowserContext();
    ExtensionRegistry* registry = ExtensionRegistry::Get(browser_context);
    std::set<std::string> extension_ids =
        ProcessMap::Get(browser_context)
            ->GetExtensionsInProcess(
                contents->GetMainFrame()->GetProcess()->GetID());
    for (std::set<std::string>::iterator iter = extension_ids.begin();
         iter != extension_ids.end(); ++iter) {
      const Extension* installed_app =
          registry->enabled_extensions().GetByID(*iter);
      if (installed_app && installed_app->is_app())
        return installed_app;
    }
    return NULL;
  }
