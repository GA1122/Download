  static int GetCSITransitionType(WebNavigationType nav_type) {
    switch (nav_type) {
      case blink::WebNavigationTypeLinkClicked:
      case blink::WebNavigationTypeFormSubmitted:
      case blink::WebNavigationTypeFormResubmitted:
        return kTransitionLink;
      case blink::WebNavigationTypeBackForward:
        return kTransitionForwardBack;
      case blink::WebNavigationTypeReload:
        return kTransitionReload;
      case blink::WebNavigationTypeOther:
        return kTransitionOther;
    }
    return kTransitionOther;
  }
