void EglRenderingVDAClient::PictureReady(const media::Picture& picture) {
  CHECK_LT(state_, CS_RESET);

  if (decoder_deleted())
    return;
  last_frame_delivered_ticks_ = base::TimeTicks::Now();

  CHECK_GE((picture.bitstream_buffer_id() + 1) * num_NALUs_per_decode_,
           num_decoded_frames_);
  CHECK_LE(picture.bitstream_buffer_id(), next_bitstream_buffer_id_);
  ++num_decoded_frames_;

  if (remaining_play_throughs_ == 1 &&
      reset_after_frame_num_ == num_decoded_frames_) {
    reset_after_frame_num_ = MID_STREAM_RESET;
    decoder_->Reset();
    encoded_data_next_pos_to_decode_ = 0;
  }

  media::PictureBuffer* picture_buffer =
      picture_buffers_by_id_[picture.picture_buffer_id()];
  CHECK(picture_buffer);
  rendering_helper_->RenderTexture(picture_buffer->texture_id());

  decoder_->ReusePictureBuffer(picture.picture_buffer_id());
}
