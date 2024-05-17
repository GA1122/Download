  ResourceRequestBlockedReason CanRequest() {
    return CanRequestInternal(SecurityViolationReportingPolicy::kReport);
  }
