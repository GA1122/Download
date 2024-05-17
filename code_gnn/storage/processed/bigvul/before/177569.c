 int SetFrameFlags(int frame_num, int num_temp_layers) {
 int frame_flags = 0;
 if (num_temp_layers == 2) {
 if (frame_num % 2 == 0) {
        frame_flags = VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_UPD_GF |
                      VP8_EFLAG_NO_UPD_ARF;
 } else {
        frame_flags = VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_LAST |
                      VP8_EFLAG_NO_UPD_ENTROPY;
 }
 } else if (num_temp_layers == 3) {
 if (frame_num % 4 == 0) {
        frame_flags = VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF |
                      VP8_EFLAG_NO_REF_GF;
 } else if ((frame_num - 2) % 4 == 0) {
        frame_flags = VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_LAST;
 } else if ((frame_num - 1) % 2 == 0) {
        frame_flags = VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF |
                      VP8_EFLAG_NO_UPD_LAST;
 }
 }
 return frame_flags;
 }
