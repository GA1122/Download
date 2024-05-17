  TestResourceMultiBuffer* test_multibuffer() {
    if (!test_multibuffer_.get()) {
      test_multibuffer_.reset(new TestResourceMultiBuffer(this, block_shift_));
    }
    return test_multibuffer_.get();
  }
