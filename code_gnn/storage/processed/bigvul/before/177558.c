 virtual void PSNRPktHook(const vpx_codec_cx_pkt_t *pkt) {
    psnr_ += pow(10.0, pkt->data.psnr.psnr[0] / 10.0);
    n_frames_++;
 }
