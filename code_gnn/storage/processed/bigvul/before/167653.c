  PendingWidgetMessageFilter()
      : BrowserMessageFilter(kMessageClasses, arraysize(kMessageClasses)),
        routing_id_(MSG_ROUTING_NONE),
        message_loop_runner_(new MessageLoopRunner) {}
