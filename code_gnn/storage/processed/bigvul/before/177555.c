   CQTest() : EncoderTest(GET_PARAM(0)), cq_level_(GET_PARAM(1)) {
     init_flags_ = VPX_CODEC_USE_PSNR;
 }
