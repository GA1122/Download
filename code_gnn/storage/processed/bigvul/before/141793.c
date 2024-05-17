void ChromeMetricsServiceClient::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  switch (type) {
    case chrome::NOTIFICATION_BROWSER_OPENED:
      UpdateRunningServices();
      metrics_service_->OnApplicationNotIdle();
      break;
    case chrome::NOTIFICATION_BROWSER_CLOSED:
    case chrome::NOTIFICATION_TAB_PARENTED:
    case chrome::NOTIFICATION_TAB_CLOSING:
    case content::NOTIFICATION_LOAD_STOP:
    case content::NOTIFICATION_LOAD_START:
    case content::NOTIFICATION_RENDERER_PROCESS_CLOSED:
    case content::NOTIFICATION_RENDER_WIDGET_HOST_HANG:
      metrics_service_->OnApplicationNotIdle();
      break;

    case chrome::NOTIFICATION_PROFILE_ADDED: {
      bool success =
          RegisterForProfileEvents(content::Source<Profile>(source).ptr());
      if (!success && notification_listeners_active_) {
        notification_listeners_active_ = false;
        UpdateRunningServices();
      }
      break;
    }
    case chrome::NOTIFICATION_PROFILE_DESTROYED:
      UpdateRunningServices();
      break;

    default:
      NOTREACHED();
  }
}
