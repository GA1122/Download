  bool AllowedScript(const Extension* extension, const GURL& url,
                     const GURL& top_url, int tab_id) {
    return extension->permissions_data()->CanAccessPage(
        extension, url, top_url, tab_id, -1, NULL);
  }
