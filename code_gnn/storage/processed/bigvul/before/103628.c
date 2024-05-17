void ChromeContentBrowserClient::RequestDesktopNotificationPermission(
    const GURL& source_origin,
    int callback_context,
    int render_process_id,
    int render_view_id) {
  RenderViewHost* rvh = RenderViewHost::FromID(
      render_process_id, render_view_id);
  if (!rvh) {
    NOTREACHED();
    return;
  }

  RenderProcessHost* process = rvh->process();
  DesktopNotificationService* service =
      DesktopNotificationServiceFactory::GetForProfile(process->profile());
  service->RequestPermission(
      source_origin, render_process_id, render_view_id, callback_context,
      tab_util::GetTabContentsByID(render_process_id, render_view_id));
}
