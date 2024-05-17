 double GetAveragePsnr() const {
 if (nframes_)
 return psnr_ / nframes_;
 return 0.0;
 }
