static void write_ivf_frame_header(const vpx_codec_cx_pkt_t *const pkt,
 FILE *const outfile) {
 char header[12];
 vpx_codec_pts_t pts;

 if (pkt->kind != VPX_CODEC_CX_FRAME_PKT)
 return;

  pts = pkt->data.frame.pts;
  mem_put_le32(header, static_cast<unsigned int>(pkt->data.frame.sz));
  mem_put_le32(header + 4, pts & 0xFFFFFFFF);
  mem_put_le32(header + 8, pts >> 32);

 (void)fwrite(header, 1, 12, outfile);
}