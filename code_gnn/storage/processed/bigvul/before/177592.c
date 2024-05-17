 virtual void MismatchHook(const vpx_image_t *img1,
 const vpx_image_t *img2) {
 double mismatch_psnr = compute_psnr(img1, img2);
    mismatch_psnr_ += mismatch_psnr;
 ++mismatch_nframes_;
 }
