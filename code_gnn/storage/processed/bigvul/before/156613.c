    WillProcessResponseObserverThrottle(content::NavigationHandle* handle,
                                        bool* will_process_response_called)
        : NavigationThrottle(handle),
          will_process_response_called_(will_process_response_called) {}
