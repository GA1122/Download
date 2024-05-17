  void TestCropFront(int pos, int size) {
    CompoundBuffer cropped;
    cropped.CopyFrom(target_, 0, target_.total_bytes());
    cropped.CropFront(pos);
    EXPECT_TRUE(CompareData(cropped, data_->data() + pos,
                            target_.total_bytes() - pos));
  }
