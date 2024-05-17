 ErrorResilienceTestLarge()
 : EncoderTest(GET_PARAM(0)),
        psnr_(0.0),
        nframes_(0),
        mismatch_psnr_(0.0),
        mismatch_nframes_(0),
        encoding_mode_(GET_PARAM(1)) {
 Reset();
 }
