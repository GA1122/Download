 virtual void PSNRPktHook(const vpx_codec_cx_pkt_t *pkt) {
 if (pkt->data.psnr.psnr[0] < psnr_)
      psnr_= pkt->data.psnr.psnr[0];
 }
