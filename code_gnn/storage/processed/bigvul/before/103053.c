  virtual void TabDetachedAt(TabContentsWrapper* contents, int index) {
    states_.push_back(new State(contents, index, DETACH));
  }
