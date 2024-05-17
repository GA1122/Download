static WebURLRequest::RequestContext DetermineRequestContextFromNavigationType(
    const NavigationType navigation_type) {
  switch (navigation_type) {
    case kNavigationTypeLinkClicked:
      return WebURLRequest::kRequestContextHyperlink;

    case kNavigationTypeOther:
      return WebURLRequest::kRequestContextLocation;

    case kNavigationTypeFormResubmitted:
    case kNavigationTypeFormSubmitted:
      return WebURLRequest::kRequestContextForm;

    case kNavigationTypeBackForward:
    case kNavigationTypeReload:
      return WebURLRequest::kRequestContextInternal;
  }
  NOTREACHED();
   return WebURLRequest::kRequestContextHyperlink;
 }
