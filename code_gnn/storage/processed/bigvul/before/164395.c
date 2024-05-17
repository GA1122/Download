  bool IsAllowedScript(const Extension* extension,
                       const GURL& url,
                       int tab_id) {
    return extension->permissions_data()->CanAccessPage(url, tab_id, nullptr);
  }
