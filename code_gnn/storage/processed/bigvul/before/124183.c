    ContentBrowserClient::CheckDesktopNotificationPermission(
        const GURL& source_origin,
        ResourceContext* context,
        int render_process_id) {
  return WebKit::WebNotificationPresenter::PermissionAllowed;
}
