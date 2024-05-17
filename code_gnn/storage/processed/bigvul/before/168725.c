DOMHighResTimeStamp PerformanceNavigationTiming::redirectEnd() const {
  bool allow_redirect_details = GetAllowRedirectDetails();
  DocumentLoadTiming* timing = GetDocumentLoadTiming();
  if (!allow_redirect_details || !timing)
    return 0;
  return Performance::MonotonicTimeToDOMHighResTimeStamp(
      TimeOrigin(), timing->RedirectEnd(), false  );
}
