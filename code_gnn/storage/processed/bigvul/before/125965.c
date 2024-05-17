MetricEventDurationObserver::MetricEventDurationObserver() {
  registrar_.Add(this, chrome::NOTIFICATION_METRIC_EVENT_DURATION,
                 content::NotificationService::AllSources());
}
