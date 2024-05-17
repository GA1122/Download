    ChromeContentBrowserClient::CheckDesktopNotificationPermission(
        const GURL& source_origin,
        content::ResourceContext* context,
        int render_process_id) {
#if defined(ENABLE_NOTIFICATIONS)
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data = ProfileIOData::FromResourceContext(context);
  if (io_data->GetExtensionInfoMap()->SecurityOriginHasAPIPermission(
        source_origin, render_process_id,
        APIPermission::kNotification))
    return WebKit::WebNotificationPresenter::PermissionAllowed;

  return io_data->GetNotificationService() ?
      io_data->GetNotificationService()->HasPermission(source_origin) :
      WebKit::WebNotificationPresenter::PermissionNotAllowed;
#else
  return WebKit::WebNotificationPresenter::PermissionAllowed;
#endif
}
