 void EncoderTest::MismatchHook(const vpx_image_t *img1,
                               const vpx_image_t *img2) {
// void EncoderTest::MismatchHook(const vpx_image_t*  ,
//                                const vpx_image_t*  ) {
    ASSERT_TRUE(0) << "Encode/Decode mismatch found";
  }