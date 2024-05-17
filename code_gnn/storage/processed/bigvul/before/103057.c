  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index) {
    State* s = new State(new_contents, index, REPLACED);
    s ->src_contents = old_contents;
    states_.push_back(s);
  }
