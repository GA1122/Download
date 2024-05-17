void OomInterventionTabHelper::RenderProcessGone(
    base::TerminationStatus status) {
  ResetInterfaces();

  if (!IsLastVisibleWebContents(web_contents())) {
    ResetInterventionState();
    return;
  }

  if (status == base::TERMINATION_STATUS_OOM_PROTECTED)
    return;

  if (near_oom_detected_time_) {
    base::TimeDelta elapsed_time =
        base::TimeTicks::Now() - near_oom_detected_time_.value();
    UMA_HISTOGRAM_MEDIUM_TIMES(
        "Memory.Experimental.OomIntervention."
        "RendererGoneAfterDetectionTime",
        elapsed_time);
    ResetInterventionState();
  } else {
    RecordNearOomDetectionEndReason(NearOomDetectionEndReason::RENDERER_GONE);
  }
}
