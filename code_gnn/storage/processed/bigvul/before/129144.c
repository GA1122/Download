  bool ScriptOnly(const Extension* extension, const GURL& url,
                  const GURL& top_url, int tab_id) {
    return AllowedScript(extension, url, top_url, tab_id) &&
           !extension->permissions_data()->CanCaptureVisiblePage(tab_id, NULL);
  }
