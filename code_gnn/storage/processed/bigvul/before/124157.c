void RenderViewHostManager::Observe(
    int type,
    const NotificationSource& source,
    const NotificationDetails& details) {
  switch (type) {
    case NOTIFICATION_RENDERER_PROCESS_CLOSING:
      RendererProcessClosing(
          Source<RenderProcessHost>(source).ptr());
      break;

    default:
      NOTREACHED();
  }
}
