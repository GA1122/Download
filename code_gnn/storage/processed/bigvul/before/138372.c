    explicit MessageLoopObserver(base::WeakPtr<IOThreadContext> context)
        : context_(context) {
      base::MessageLoop::current()->AddDestructionObserver(this);
    }
