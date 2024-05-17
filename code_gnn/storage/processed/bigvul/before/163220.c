  void Continue(NavigationThrottle::ThrottleCheckResult result) {
    ASSERT_NE(NavigationThrottle::DEFER, result.action());
    if (result.action() == NavigationThrottle::PROCEED)
      navigation_throttle()->ResumeNavigation();
    else
      navigation_throttle()->CancelNavigation(result);
  }
