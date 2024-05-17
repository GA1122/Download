static bool ShouldNavigateTargetFrame(NavigationPolicy policy) {
  switch (policy) {
    case kNavigationPolicyCurrentTab:
      return true;

    case kNavigationPolicyNewBackgroundTab:
    case kNavigationPolicyNewForegroundTab:
    case kNavigationPolicyNewWindow:
    case kNavigationPolicyNewPopup:
      return false;

    case kNavigationPolicyIgnore:
    case kNavigationPolicyDownload:
      return false;

    case kNavigationPolicyHandledByClient:
      NOTREACHED();
      return true;

    default:
      NOTREACHED() << policy;
      return true;
  }
}
