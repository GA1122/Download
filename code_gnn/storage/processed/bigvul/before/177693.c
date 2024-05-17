 void UpdateMD5(::libvpx_test::Decoder *dec, const vpx_codec_cx_pkt_t *pkt,
 ::libvpx_test::MD5 *md5) {
 const vpx_codec_err_t res = dec->DecodeFrame(
 reinterpret_cast<uint8_t*>(pkt->data.frame.buf), pkt->data.frame.sz);
 if (res != VPX_CODEC_OK) {
      abort_ = true;
      ASSERT_EQ(VPX_CODEC_OK, res);
 }
 const vpx_image_t *img = dec->GetDxData().Next();
    md5->Add(img);
 }
