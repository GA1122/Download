 virtual void PreEncodeFrameHook(::libvpx_test::VideoSource *video,
 ::libvpx_test::Encoder *encoder) {
 if (video->frame() == 1) {
      encoder->Control(VP8E_SET_CPUUSED, set_cpu_used_);
      encoder->Control(VP9E_SET_AQ_MODE, aq_mode_);
      encoder->Control(VP8E_SET_MAX_INTRA_BITRATE_PCT, 100);

     }
   }
