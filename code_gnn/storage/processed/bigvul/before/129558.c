void CorePageLoadMetricsObserver::OnDomContentLoadedEventStart(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (WasStartedInForegroundOptionalEventInForeground(
          timing.dom_content_loaded_event_start, info)) {
    PAGE_LOAD_HISTOGRAM(internal::kHistogramDomContentLoadedImmediate,
                        timing.dom_content_loaded_event_start.value());
  } else {
    PAGE_LOAD_HISTOGRAM(internal::kBackgroundHistogramDomContentLoadedImmediate,
                        timing.dom_content_loaded_event_start.value());
  }
}
