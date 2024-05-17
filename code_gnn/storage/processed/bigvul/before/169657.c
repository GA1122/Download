  static std::unique_ptr<NavigationThrottle> Create(NavigationHandle* handle) {
    return std::make_unique<RequestBlockingNavigationThrottle>(handle);
  }
