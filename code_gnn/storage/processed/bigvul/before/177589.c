 double GetAverageMismatchPsnr() const {
 if (mismatch_nframes_)
 return mismatch_psnr_ / mismatch_nframes_;
 return 0.0;
 }
