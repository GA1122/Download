 virtual void PSNRPktHook(const vpx_codec_cx_pkt_t *pkt) {
 if (!frame0_psnr_)
      frame0_psnr_ = pkt->data.psnr.psnr[0];

     EXPECT_NEAR(pkt->data.psnr.psnr[0], frame0_psnr_, 2.0);
   }
