  TestNavigationThrottle(
      NavigationHandle* handle,
      NavigationThrottle::ThrottleCheckResult will_start_result,
      NavigationThrottle::ThrottleCheckResult will_redirect_result,
      NavigationThrottle::ThrottleCheckResult will_fail_result,
      NavigationThrottle::ThrottleCheckResult will_process_result,
      base::Closure did_call_will_start,
      base::Closure did_call_will_redirect,
      base::Closure did_call_will_fail,
      base::Closure did_call_will_process)
      : NavigationThrottle(handle),
        will_start_result_(will_start_result),
        will_redirect_result_(will_redirect_result),
        will_fail_result_(will_fail_result),
        will_process_result_(will_process_result),
        did_call_will_start_(did_call_will_start),
        did_call_will_redirect_(did_call_will_redirect),
        did_call_will_fail_(did_call_will_fail),
        did_call_will_process_(did_call_will_process) {}
