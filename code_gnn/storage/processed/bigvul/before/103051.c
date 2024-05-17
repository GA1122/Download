  virtual void TabChangedAt(TabContentsWrapper* contents, int index,
                            TabChangeType change_type) {
    states_.push_back(new State(contents, index, CHANGE));
  }
