 virtual void PreEncodeFrameHook(libvpx_test::VideoSource *video,
                                  libvpx_test::Encoder *encoder) {
 if (video->frame() == 1) {
      encoder->Control(VP9E_SET_TILE_COLUMNS, n_tiles_);
 }
 }
