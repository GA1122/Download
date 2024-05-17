void PageHandler::Observe(int type,
                          const NotificationSource& source,
                          const NotificationDetails& details) {
  if (!screencast_enabled_)
    return;
  DCHECK(type == content::NOTIFICATION_RENDER_WIDGET_VISIBILITY_CHANGED);
  bool visible = *Details<bool>(details).ptr();
  NotifyScreencastVisibility(visible);
}
