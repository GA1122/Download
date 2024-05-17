  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            TabContentsWrapper* contents,
                            int index) {
    states_.push_back(new State(contents, index, CLOSE));
  }
