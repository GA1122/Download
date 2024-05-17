void CorePageLoadMetricsObserver::OnParseStop(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  base::TimeDelta parse_duration =
      timing.parse_stop.value() - timing.parse_start.value();
  if (WasStartedInForegroundOptionalEventInForeground(timing.parse_stop,
                                                      info)) {
    PAGE_LOAD_HISTOGRAM(internal::kHistogramParseDurationImmediate,
                        parse_duration);
    PAGE_LOAD_HISTOGRAM(internal::kHistogramParseBlockedOnScriptLoadImmediate,
                        timing.parse_blocked_on_script_load_duration.value());
    PAGE_LOAD_HISTOGRAM(
        internal::kHistogramParseBlockedOnScriptLoadDocumentWriteImmediate,
        timing.parse_blocked_on_script_load_from_document_write_duration
            .value());
  } else {
    PAGE_LOAD_HISTOGRAM(internal::kBackgroundHistogramParseDurationImmediate,
                        parse_duration);
    PAGE_LOAD_HISTOGRAM(
        internal::kBackgroundHistogramParseBlockedOnScriptLoadImmediate,
        timing.parse_blocked_on_script_load_duration.value());
    PAGE_LOAD_HISTOGRAM(
        internal::
            kBackgroundHistogramParseBlockedOnScriptLoadDocumentWriteImmediate,
        timing.parse_blocked_on_script_load_from_document_write_duration
            .value());
  }
}
