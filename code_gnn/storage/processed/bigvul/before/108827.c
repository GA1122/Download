  ~RenderViewHostObserverArray() {
    for (std::list<RVHObserver*>::iterator iter = observers_.begin();
         iter != observers_.end(); ++iter) {
      (*iter)->ClearParent();
    }
  }
