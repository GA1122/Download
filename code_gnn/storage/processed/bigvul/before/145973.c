  int GetPreCountAndReset() {
    int r = pre_count_;
    pre_count_ = 0;
    return r;
  }
