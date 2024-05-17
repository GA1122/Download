  MetricsStateManagerTest() : is_metrics_reporting_enabled_(false) {
    MetricsService::RegisterPrefs(prefs_.registry());
  }
