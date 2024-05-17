void OomInterventionTabHelper::OnCrashDumpProcessed(
    int rph_id,
    const crash_reporter::CrashMetricsReporter::ReportedCrashTypeSet&
        reported_counts) {
  if (rph_id != web_contents()->GetMainFrame()->GetProcess()->GetID())
    return;
  if (!reported_counts.count(
          crash_reporter::CrashMetricsReporter::ProcessedCrashCounts::
              kRendererForegroundVisibleOom)) {
    return;
  }

  DCHECK(IsLastVisibleWebContents(web_contents()));
  if (near_oom_detected_time_) {
    base::TimeDelta elapsed_time =
        base::TimeTicks::Now() - near_oom_detected_time_.value();
    UMA_HISTOGRAM_MEDIUM_TIMES(
        "Memory.Experimental.OomIntervention."
        "OomProtectedCrashAfterDetectionTime",
        elapsed_time);

    if (intervention_state_ != InterventionState::NOT_TRIGGERED) {
      bool accepted = intervention_state_ != InterventionState::DECLINED;
      RecordInterventionStateOnCrash(accepted);
    }
    ResetInterventionState();
  } else {
    RecordNearOomDetectionEndReason(
        NearOomDetectionEndReason::OOM_PROTECTED_CRASH);
  }

  base::TimeDelta time_since_last_navigation;
  if (!last_navigation_timestamp_.is_null()) {
    time_since_last_navigation =
        base::TimeTicks::Now() - last_navigation_timestamp_;
  }
  UMA_HISTOGRAM_COUNTS_1M(
      "Memory.Experimental.OomIntervention."
      "RendererTimeSinceLastNavigationAtOOM",
      time_since_last_navigation.InSeconds());

  if (decider_) {
    DCHECK(!web_contents()->GetBrowserContext()->IsOffTheRecord());
    const std::string& host = web_contents()->GetVisibleURL().host();
    decider_->OnOomDetected(host);
  }
}
