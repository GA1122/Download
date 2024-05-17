  void SwapPreferredSizeTo(bool use_a) {
    if (use_a)
      SetPreferredSize(a_);
    else
      SetPreferredSize(b_);
  }
