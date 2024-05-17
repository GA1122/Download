  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground) {
    empty_ = false;
    State* s = new State(contents, index, INSERT);
    s->foreground = foreground;
    states_.push_back(s);
  }
