  void AppendRestore(size_t n) {
    cc_list_.StartPaint();
    while (n--)
      cc_list_.push<cc::RestoreOp>();
    cc_list_.EndPaintOfPairedEnd();
  }
