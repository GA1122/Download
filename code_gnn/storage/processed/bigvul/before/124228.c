  WindowedLoadStopObserver(NavigationController* controller,
                           int notification_count)
      : WindowedNavigationObserver<content::NOTIFICATION_LOAD_STOP>(controller),
        remaining_notification_count_(notification_count) {}
