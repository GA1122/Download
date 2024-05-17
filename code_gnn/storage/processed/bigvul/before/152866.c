UkmPageLoadMetricsObserver::ObservePolicy UkmPageLoadMetricsObserver::OnHidden(
    const page_load_metrics::mojom::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (!was_hidden_) {
    RecordPageLoadExtraInfoMetrics(
        info, base::TimeTicks()  );
    RecordTimingMetrics(timing, info);
    was_hidden_ = true;
  }
  return CONTINUE_OBSERVING;
}
