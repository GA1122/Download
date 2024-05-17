  explicit WindowedNavigationObserver(NavigationController* controller)
      : content::WindowedNotificationObserver(
          T, content::Source<NavigationController>(controller)) {}
