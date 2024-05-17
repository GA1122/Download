 virtual const vpx_codec_cx_pkt_t * MutateEncoderOutputHook(
 const vpx_codec_cx_pkt_t *pkt) {
 if (pkt->kind != VPX_CODEC_CX_FRAME_PKT)
 return pkt;

 const uint8_t *buffer = reinterpret_cast<uint8_t*>(pkt->data.frame.buf);
 const uint8_t marker = buffer[pkt->data.frame.sz - 1];
 const int frames = (marker & 0x7) + 1;
 const int mag = ((marker >> 3) & 3) + 1;
 const unsigned int index_sz = 2 + mag  * frames;
 if ((marker & 0xe0) == 0xc0 &&
        pkt->data.frame.sz >= index_sz &&
        buffer[pkt->data.frame.sz - index_sz] == marker) {
 if (modified_buf_)
 delete[] modified_buf_;
      modified_buf_ = new uint8_t[pkt->data.frame.sz - index_sz];
      memcpy(modified_buf_, pkt->data.frame.buf,
             pkt->data.frame.sz - index_sz);
      modified_pkt_ = *pkt;
      modified_pkt_.data.frame.buf = modified_buf_;
      modified_pkt_.data.frame.sz -= index_sz;

      sf_count_++;
      last_sf_pts_ = pkt->data.frame.pts;
 return &modified_pkt_;
 }

    abort_ |= sf_count_ > sf_count_max_ &&
              pkt->data.frame.pts - last_sf_pts_ >= 5;
 return pkt;
 }
