 virtual void FramePktHook(const vpx_codec_cx_pkt_t *pkt) {
 vpx_codec_pts_t duration = pkt->data.frame.pts - last_pts_;

 if (last_pts_ == 0)
      duration = 1;

    bits_in_buffer_model_ += static_cast<int64_t>(
        duration * timebase_ * cfg_.rc_target_bitrate * 1000);

  
 const bool key_frame = (pkt->data.frame.flags & VPX_FRAME_IS_KEY)
 ? true: false;
 if (!key_frame) {
      ASSERT_GE(bits_in_buffer_model_, 0) << "Buffer Underrun at frame "
 << pkt->data.frame.pts;
 }

 const size_t frame_size_in_bits = pkt->data.frame.sz * 8;

    bits_in_buffer_model_ -= frame_size_in_bits;

    bits_total_ += frame_size_in_bits;

 if (!first_drop_ && duration > 1)
      first_drop_ = last_pts_ + 1;

    last_pts_ = pkt->data.frame.pts;

    bits_in_last_frame_ = frame_size_in_bits;

 ++frame_number_;
 }
