  int GetPostCountAndReset() {
    int r = post_count_;
    post_count_ = 0;
    return r;
  }
