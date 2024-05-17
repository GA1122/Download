  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture) {
    State* s = new State(new_contents, index, SELECT);
    s->src_contents = old_contents;
    s->user_gesture = user_gesture;
    states_.push_back(s);
  }
