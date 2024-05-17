void ChromeContentBrowserClient::ShowDesktopNotification(
    const DesktopNotificationHostMsg_Show_Params& params,
    int render_process_id,
    int render_view_id,
    bool worker) {
  RenderViewHost* rvh = RenderViewHost::FromID(
      render_process_id, render_view_id);
  if (!rvh) {
    NOTREACHED();
    return;
  }

  RenderProcessHost* process = rvh->process();
  DesktopNotificationService* service =
      DesktopNotificationServiceFactory::GetForProfile(process->profile());
  service->ShowDesktopNotification(
    params, render_process_id, render_view_id,
    worker ? DesktopNotificationService::WorkerNotification :
        DesktopNotificationService::PageNotification);
}
