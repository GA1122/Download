UkmPageLoadMetricsObserver::FlushMetricsOnAppEnterBackground(
    const page_load_metrics::mojom::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (!was_hidden_) {
    RecordPageLoadExtraInfoMetrics(info, base::TimeTicks::Now());
    RecordTimingMetrics(timing, info);
  }
  ReportLayoutStability(info);
  return STOP_OBSERVING;
}
