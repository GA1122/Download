void Browser::OpenTaskManager() {
  UserMetrics::RecordAction(UserMetricsAction("TaskManager"), profile_);
  window_->ShowTaskManager();
}
