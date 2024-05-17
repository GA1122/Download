  void AddObserverToRVH(RenderViewHost* rvh) {
    observers_.push_back(new RVHObserver(this, rvh));
  }
