    ChromeContentBrowserClient::CheckDesktopNotificationPermission(
        const GURL& source_url,
        const content::ResourceContext& context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data =
      reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));

  const Extension* extension =
      io_data->GetExtensionInfoMap()->extensions().GetByURL(source_url);
  if (extension &&
      extension->HasAPIPermission(ExtensionAPIPermission::kNotification)) {
    return WebKit::WebNotificationPresenter::PermissionAllowed;
  }

  return io_data->GetNotificationService() ?
      io_data->GetNotificationService()->HasPermission(source_url.GetOrigin()) :
      WebKit::WebNotificationPresenter::PermissionNotAllowed;
}
