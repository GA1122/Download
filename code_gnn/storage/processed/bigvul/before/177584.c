 void Encoder::EncodeFrame(VideoSource *video, const unsigned long frame_flags) {
   if (video->img())
     EncodeFrameInternal(*video, frame_flags);
 else
 Flush();

 CxDataIterator iter = GetCxData();

 while (const vpx_codec_cx_pkt_t *pkt = iter.Next()) {
 if (pkt->kind != VPX_CODEC_STATS_PKT)
 continue;

    stats_->Append(*pkt);
 }
}
