 virtual void FramePktHook(const vpx_codec_cx_pkt_t *pkt) {
 vpx_codec_pts_t duration = pkt->data.frame.pts - last_pts_;

 if (duration > 1) {
 if (!first_drop_)
        first_drop_ = last_pts_ + 1;
      num_drops_ += static_cast<int>(duration - 1);
      tot_frame_number_ += static_cast<int>(duration - 1);
 }

 int layer = SetLayerId(tot_frame_number_, cfg_.ts_number_layers);

    bits_in_buffer_model_ += static_cast<int64_t>(
        duration * timebase_ * cfg_.rc_target_bitrate * 1000);

    ASSERT_GE(bits_in_buffer_model_, 0) << "Buffer Underrun at frame "
 << pkt->data.frame.pts;

 const size_t frame_size_in_bits = pkt->data.frame.sz * 8;

 for (int i = layer; i < static_cast<int>(cfg_.ts_number_layers); ++i) {
      bits_total_[i] += frame_size_in_bits;
 }

    last_pts_ = pkt->data.frame.pts;
 ++frame_number_;
 ++tot_frame_number_;
 }
