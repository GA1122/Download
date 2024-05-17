 virtual void EndPassHook(void) {
 if (bits_total_) {
 const double file_size_in_kb = bits_total_ / 1000.;  

      duration_ = (last_pts_ + 1) * timebase_;

      effective_datarate_ = (bits_total_ - bits_in_last_frame_) / 1000.0
 / (cfg_.rc_buf_initial_sz / 1000.0 + duration_);

      file_datarate_ = file_size_in_kb / duration_;
 }
 }
