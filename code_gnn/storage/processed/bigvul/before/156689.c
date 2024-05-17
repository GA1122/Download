  PendingWidgetMessageFilter()
      : BrowserMessageFilter(kMessageClasses, arraysize(kMessageClasses)),
        routing_id_(MSG_ROUTING_NONE) {}
