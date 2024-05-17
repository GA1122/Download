static WebURLRequest::RequestContext DetermineRequestContextFromNavigationType(
    const WebNavigationType navigation_type) {
  switch (navigation_type) {
    case kWebNavigationTypeLinkClicked:
      return WebURLRequest::kRequestContextHyperlink;

    case kWebNavigationTypeOther:
      return WebURLRequest::kRequestContextLocation;

    case kWebNavigationTypeFormResubmitted:
    case kWebNavigationTypeFormSubmitted:
      return WebURLRequest::kRequestContextForm;

    case kWebNavigationTypeBackForward:
    case kWebNavigationTypeReload:
      return WebURLRequest::kRequestContextInternal;
  }
  NOTREACHED();
  return WebURLRequest::kRequestContextHyperlink;
}
