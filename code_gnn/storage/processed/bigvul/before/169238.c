  explicit FrameTreeNodeObserverImpl(FrameTreeNode* owner)
      : owner_(owner), message_loop_runner_(new MessageLoopRunner) {
    owner->AddObserver(this);
  }
