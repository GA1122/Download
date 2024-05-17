 virtual void FramePktHook(const vpx_codec_cx_pkt_t *pkt) {
 UpdateMD5(fw_dec_, pkt, &md5_fw_order_);
 UpdateMD5(inv_dec_, pkt, &md5_inv_order_);
 }
