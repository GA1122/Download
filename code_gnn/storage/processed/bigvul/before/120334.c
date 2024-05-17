  explicit QueueTouchEventDelegate(WindowEventDispatcher* dispatcher)
      : window_(NULL),
        dispatcher_(dispatcher),
        queue_events_(true) {
  }
