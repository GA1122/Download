void ResourceMessageFilter::OnCheckNotificationPermission(
    const GURL& source_url, int* result) {
  *result = WebKit::WebNotificationPresenter::PermissionNotAllowed;

  ChromeURLRequestContext* context = GetRequestContextForURL(source_url);
  if (context->CheckURLAccessToExtensionPermission(source_url,
      Extension::kNotificationPermission)) {
    *result = WebKit::WebNotificationPresenter::PermissionAllowed;
    return;
  }

  *result = notification_prefs_->HasPermission(source_url.GetOrigin());
}
