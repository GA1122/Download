 virtual void PreEncodeFrameHook(libvpx_test::VideoSource *video,
                                  libvpx_test::Encoder *encoder) {
 if (video->frame() == 1) {
      encoder->Control(VP8E_SET_ENABLEAUTOALTREF, 1);
 }
 }
