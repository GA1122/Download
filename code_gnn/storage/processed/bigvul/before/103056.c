  virtual void TabPinnedStateChanged(TabContentsWrapper* contents, int index) {
    states_.push_back(new State(contents, index, PINNED));
  }
