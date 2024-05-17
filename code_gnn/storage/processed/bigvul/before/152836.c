void MetricsWebContentsObserver::NotifyPageEndAllLoads(
    PageEndReason page_end_reason,
    UserInitiatedInfo user_initiated_info) {
  NotifyPageEndAllLoadsWithTimestamp(page_end_reason, user_initiated_info,
                                     base::TimeTicks::Now(), true);
}
