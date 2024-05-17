void Browser::OpenTaskManager(bool highlight_background_resources) {
  UserMetrics::RecordAction(UserMetricsAction("TaskManager"));
  if (highlight_background_resources)
    window_->ShowBackgroundPages();
  else
    window_->ShowTaskManager();
}
