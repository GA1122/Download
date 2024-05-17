bool AreActiveNotificationProcessesReady() {
  NotificationUIManager* manager = g_browser_process->notification_ui_manager();
  const BalloonCollection::Balloons& balloons =
      manager->balloon_collection()->GetActiveBalloons();
  BalloonCollection::Balloons::const_iterator iter;
  for (iter = balloons.begin(); iter != balloons.end(); ++iter) {
    BalloonHost* balloon_host = (*iter)->balloon_view()->GetHost();
    if (balloon_host && !balloon_host->IsRenderViewReady())
      return false;
  }
  return true;
}
