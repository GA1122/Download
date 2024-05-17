static mojom::RequestContextType DetermineRequestContextFromNavigationType(
    const WebNavigationType navigation_type) {
  switch (navigation_type) {
    case kWebNavigationTypeLinkClicked:
      return mojom::RequestContextType::HYPERLINK;

    case kWebNavigationTypeOther:
      return mojom::RequestContextType::LOCATION;

    case kWebNavigationTypeFormResubmitted:
    case kWebNavigationTypeFormSubmitted:
      return mojom::RequestContextType::FORM;

    case kWebNavigationTypeBackForward:
    case kWebNavigationTypeReload:
      return mojom::RequestContextType::INTERNAL;
  }
  NOTREACHED();
  return mojom::RequestContextType::HYPERLINK;
}
