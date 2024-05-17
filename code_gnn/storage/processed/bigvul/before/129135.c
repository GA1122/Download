  bool BlockedScript(const Extension* extension, const GURL& url,
                     const GURL& top_url) {
    return !extension->permissions_data()->CanAccessPage(
        extension, url, top_url, -1, -1, NULL);
  }
