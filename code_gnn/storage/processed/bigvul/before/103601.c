void ChromeContentBrowserClient::CancelDesktopNotification(
    int render_process_id,
    int render_view_id,
    int notification_id) {
  RenderViewHost* rvh = RenderViewHost::FromID(
      render_process_id, render_view_id);
  if (!rvh) {
    NOTREACHED();
    return;
  }

  RenderProcessHost* process = rvh->process();
  DesktopNotificationService* service =
      DesktopNotificationServiceFactory::GetForProfile(process->profile());
  service->CancelDesktopNotification(
      render_process_id, render_view_id, notification_id);
}
