 virtual bool DoDecode() const {
 if (error_nframes_ > 0 &&
 (cfg_.g_pass == VPX_RC_LAST_PASS || cfg_.g_pass == VPX_RC_ONE_PASS)) {
 for (unsigned int i = 0; i < error_nframes_; ++i) {
 if (error_frames_[i] == nframes_ - 1) {
          std::cout << "             Skipping decoding frame: "
 << error_frames_[i] << "\n";
 return 0;
 }
 }
 }
 return 1;
 }
