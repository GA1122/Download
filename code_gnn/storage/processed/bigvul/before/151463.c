  ResourceRequestBlockedReason CanRequestPreload() {
    return CanRequestInternal(
        SecurityViolationReportingPolicy::kSuppressReporting);
  }
