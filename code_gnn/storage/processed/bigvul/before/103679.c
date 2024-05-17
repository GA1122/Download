void BrowserRenderProcessHost::OnShutdownRequest() {
  if (pending_views_)
    return;

  NotificationService::current()->Notify(
      content::NOTIFICATION_RENDERER_PROCESS_CLOSING,
      Source<RenderProcessHost>(this), NotificationService::NoDetails());

  Send(new ChildProcessMsg_Shutdown());
}
