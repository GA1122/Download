  explicit DocumentOutliveTimeReporter(Document* document)
      : BlinkGCObserver(ThreadState::Current()), document_(document) {}
