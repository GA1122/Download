void RenderMessageFilter::OnCheckNotificationPermission(
    const GURL& source_origin, int* result) {
#if defined(ENABLE_NOTIFICATIONS)
  *result = GetContentClient()->browser()->
      CheckDesktopNotificationPermission(source_origin, resource_context_,
                                         render_process_id_);
#else
  *result = WebKit::WebNotificationPresenter::PermissionAllowed;
#endif
}
