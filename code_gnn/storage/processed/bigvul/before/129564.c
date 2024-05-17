void CorePageLoadMetricsObserver::OnFirstTextPaint(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (WasStartedInForegroundOptionalEventInForeground(timing.first_text_paint,
                                                      info)) {
    PAGE_LOAD_HISTOGRAM(internal::kHistogramFirstTextPaintImmediate,
                        timing.first_text_paint.value());
  } else {
    PAGE_LOAD_HISTOGRAM(internal::kBackgroundHistogramFirstTextPaintImmediate,
                        timing.first_text_paint.value());
  }
}
