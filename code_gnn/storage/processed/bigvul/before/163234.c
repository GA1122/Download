  TestNavigationThrottleInstaller(
      WebContents* web_contents,
      NavigationThrottle::ThrottleCheckResult will_start_result,
      NavigationThrottle::ThrottleCheckResult will_redirect_result,
      NavigationThrottle::ThrottleCheckResult will_fail_result,
      NavigationThrottle::ThrottleCheckResult will_process_result,
      const GURL& expected_start_url = GURL())
      : WebContentsObserver(web_contents),
        will_start_result_(will_start_result),
        will_redirect_result_(will_redirect_result),
        will_fail_result_(will_fail_result),
        will_process_result_(will_process_result),
        expected_start_url_(expected_start_url),
        weak_factory_(this) {}
