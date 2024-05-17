 virtual void SetUp() {
    pred_fn_ = GetParam();
 SetupMacroblock(mb_, mi_, data_array_, kBlockSize, kStride, 2);
 }
