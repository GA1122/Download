   bool IsAllowedScript(const Extension* extension,
                       const GURL& url,
                       int tab_id) {
    return extension->permissions_data()->CanAccessPage(extension, url, tab_id,
                                                        nullptr);
  }
