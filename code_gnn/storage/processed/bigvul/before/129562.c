void CorePageLoadMetricsObserver::OnFirstLayout(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (WasStartedInForegroundOptionalEventInForeground(timing.first_layout,
                                                      info)) {
    PAGE_LOAD_HISTOGRAM(internal::kHistogramFirstLayoutImmediate,
                        timing.first_layout.value());
  } else {
    PAGE_LOAD_HISTOGRAM(internal::kBackgroundHistogramFirstLayoutImmediate,
                        timing.first_layout.value());
  }
}
