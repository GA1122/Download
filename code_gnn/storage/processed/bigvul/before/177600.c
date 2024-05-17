 vpx_codec_err_t DecodeOneFrame() {
 const vpx_codec_err_t res =
        decoder_->DecodeFrame(video_->cxdata(), video_->frame_size());
 CheckDecodedFrames();
 if (res == VPX_CODEC_OK)
      video_->Next();
 return res;
 }
