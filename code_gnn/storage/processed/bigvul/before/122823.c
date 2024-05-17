  void CreateSharedContextLazy() {
    if (shared_context_.get())
      return;

    shared_context_.reset(CreateOffscreenContext());
    if (!shared_context_.get()) {
      LOG(FATAL) << "Failed to initialize UI shared context.";
    }
    if (!shared_context_->makeContextCurrent()) {
      LOG(FATAL) << "Failed to make UI shared context current.";
    }
    shared_context_->setContextLostCallback(this);
  }
