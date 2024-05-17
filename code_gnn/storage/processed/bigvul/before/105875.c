  void TestCropBack(int pos, int size) {
    CompoundBuffer cropped;
    cropped.CopyFrom(target_, 0, target_.total_bytes());
    cropped.CropBack(pos);
    EXPECT_TRUE(CompareData(cropped, data_->data(),
                            target_.total_bytes() - pos));
  }
