  NetworkNavigationThrottle(
      base::WeakPtr<protocol::NetworkHandler> network_handler,
      content::NavigationHandle* navigation_handle)
      : content::NavigationThrottle(navigation_handle),
        network_handler_(network_handler) {}
