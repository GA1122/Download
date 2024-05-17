void EncoderTest::SetMode(TestMode mode) {
 switch (mode) {
 case kRealTime:
      deadline_ = VPX_DL_REALTIME;
 break;

 case kOnePassGood:
 case kTwoPassGood:
      deadline_ = VPX_DL_GOOD_QUALITY;
 break;

 case kOnePassBest:
 case kTwoPassBest:
      deadline_ = VPX_DL_BEST_QUALITY;
 break;

 default:
      ASSERT_TRUE(false) << "Unexpected mode " << mode;
 }

 if (mode == kTwoPassGood || mode == kTwoPassBest)
    passes_ = 2;
 else
    passes_ = 1;
}
