  virtual void SetUp() {
    data_ = new IOBuffer(kDataSize);
    for (int i = 0; i < kDataSize; ++i) {
      data_->data()[i] = i;
    }
  }
