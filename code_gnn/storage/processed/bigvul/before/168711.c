AtomicString PerformanceNavigationTiming::GetNavigationType(
    NavigationType type,
    const Document* document) {
  if (document && document->GetPageVisibilityState() ==
                      mojom::PageVisibilityState::kPrerender) {
    return "prerender";
  }
  switch (type) {
    case kNavigationTypeReload:
      return "reload";
    case kNavigationTypeBackForward:
      return "back_forward";
    case kNavigationTypeLinkClicked:
    case kNavigationTypeFormSubmitted:
    case kNavigationTypeFormResubmitted:
    case kNavigationTypeOther:
      return "navigate";
  }
  NOTREACHED();
  return "navigate";
}
