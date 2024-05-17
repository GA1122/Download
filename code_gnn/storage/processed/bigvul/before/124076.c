bool ChromeContentBrowserClient::CanCreateWindow(
    const GURL& opener_url,
    const GURL& source_origin,
    WindowContainerType container_type,
    content::ResourceContext* context,
    int render_process_id,
    bool* no_javascript_access) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  *no_javascript_access = false;

  if (container_type == WINDOW_CONTAINER_TYPE_BACKGROUND) {
    ProfileIOData* io_data = ProfileIOData::FromResourceContext(context);
    ExtensionInfoMap* map = io_data->GetExtensionInfoMap();

    if (!map->SecurityOriginHasAPIPermission(
            source_origin,
            render_process_id,
            APIPermission::kBackground)) {
      return false;
    }

    const Extension* extension = map->extensions().GetExtensionOrAppByURL(
        ExtensionURLInfo(opener_url));
    if (extension && !extension->allow_background_js_access())
      *no_javascript_access = true;
  }
  return true;
}
