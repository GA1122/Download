  explicit FrameTreeNodeObserverImpl(FrameTreeNode* owner) : owner_(owner) {
    owner->AddObserver(this);
  }
