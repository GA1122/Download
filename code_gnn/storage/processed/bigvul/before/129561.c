void CorePageLoadMetricsObserver::OnFirstImagePaint(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (WasStartedInForegroundOptionalEventInForeground(timing.first_image_paint,
                                                      info)) {
    PAGE_LOAD_HISTOGRAM(internal::kHistogramFirstImagePaintImmediate,
                        timing.first_image_paint.value());
  } else {
    PAGE_LOAD_HISTOGRAM(internal::kBackgroundHistogramFirstImagePaintImmediate,
                        timing.first_image_paint.value());
  }
}
