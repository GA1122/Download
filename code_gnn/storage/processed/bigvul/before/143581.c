void OomInterventionTabHelper::StartMonitoringIfNeeded() {
  if (subscription_)
    return;

  if (intervention_)
    return;

  if (near_oom_detected_time_)
    return;

  auto* config = OomInterventionConfig::GetInstance();
  if (config->should_detect_in_renderer()) {
    if (binding_.is_bound())
      return;
    StartDetectionInRenderer();
  } else if (config->is_swap_monitor_enabled()) {
    subscription_ = NearOomMonitor::GetInstance()->RegisterCallback(
        base::BindRepeating(&OomInterventionTabHelper::OnNearOomDetected,
                            base::Unretained(this)));
  }
}
