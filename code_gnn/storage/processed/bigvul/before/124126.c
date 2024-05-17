void ChromeContentBrowserClient::RequestDesktopNotificationPermission(
    const GURL& source_origin,
    int callback_context,
    int render_process_id,
    int render_view_id) {
#if defined(ENABLE_NOTIFICATIONS)
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  WebContents* contents =
      tab_util::GetWebContentsByID(render_process_id, render_view_id);
  if (!contents) {
    NOTREACHED();
    return;
  }

  Profile* profile = Profile::FromBrowserContext(contents->GetBrowserContext());
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  const Extension* extension = !service ? NULL :
      service->extensions()->GetExtensionOrAppByURL(ExtensionURLInfo(
          source_origin));
  RenderViewHost* rvh =
      RenderViewHost::FromID(render_process_id, render_view_id);
  if (IsExtensionWithPermissionOrSuggestInConsole(
      APIPermission::kNotification, extension, rvh)) {
    if (rvh)
      rvh->DesktopNotificationPermissionRequestDone(callback_context);
    return;
  }

  DesktopNotificationService* notifications =
      DesktopNotificationServiceFactory::GetForProfile(profile);
  notifications->RequestPermission(source_origin, render_process_id,
      render_view_id, callback_context, contents);
#else
  NOTIMPLEMENTED();
#endif
}
