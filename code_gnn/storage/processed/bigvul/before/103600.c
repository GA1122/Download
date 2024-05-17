bool ChromeContentBrowserClient::CanCreateWindow(
    const GURL& source_url,
    WindowContainerType container_type,
    const content::ResourceContext& context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (container_type == WINDOW_CONTAINER_TYPE_BACKGROUND) {
    ProfileIOData* io_data =
        reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));
    const Extension* extension =
        io_data->GetExtensionInfoMap()->extensions().GetByURL(source_url);
    return (extension &&
            extension->HasAPIPermission(ExtensionAPIPermission::kBackground));
  }
  return true;
}
