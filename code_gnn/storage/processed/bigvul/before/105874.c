  void TestCopyFrom(int pos, int size) {
    CompoundBuffer copy;
    copy.CopyFrom(target_, pos, pos + size);
    EXPECT_TRUE(CompareData(copy, data_->data() + pos, size));
  }
