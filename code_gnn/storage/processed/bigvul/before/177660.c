 virtual void PreEncodeFrameHook(libvpx_test::VideoSource *video,
                                  libvpx_test::Encoder *encoder) {
 if (video->frame() == kStepDownFrame) {
 struct vpx_scaling_mode mode = {VP8E_FOURFIVE, VP8E_THREEFIVE};
      encoder->Control(VP8E_SET_SCALEMODE, &mode);
 }
 if (video->frame() == kStepUpFrame) {
 struct vpx_scaling_mode mode = {VP8E_NORMAL, VP8E_NORMAL};
      encoder->Control(VP8E_SET_SCALEMODE, &mode);
 }
 }
