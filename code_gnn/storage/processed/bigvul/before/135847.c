  static const char* GetNavigationType(WebNavigationType nav_type) {
    switch (nav_type) {
      case blink::WebNavigationTypeLinkClicked:
        return "LinkClicked";
      case blink::WebNavigationTypeFormSubmitted:
        return "FormSubmitted";
      case blink::WebNavigationTypeBackForward:
        return "BackForward";
      case blink::WebNavigationTypeReload:
        return "Reload";
      case blink::WebNavigationTypeFormResubmitted:
        return "Resubmitted";
      case blink::WebNavigationTypeOther:
        return "Other";
    }
    return "";
  }
