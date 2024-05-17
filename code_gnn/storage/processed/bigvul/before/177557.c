 double GetLinearPSNROverBitrate() const {
 double avg_psnr = log10(psnr_ / n_frames_) * 10.0;

     return pow(10.0, avg_psnr / 10.0) / file_size_;
   }
