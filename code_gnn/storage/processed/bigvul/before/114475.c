void EglRenderingVDAClient::NotifyResetDone() {
  if (decoder_deleted())
    return;

  if (reset_after_frame_num_ == MID_STREAM_RESET) {
    reset_after_frame_num_ = END_OF_STREAM_RESET;
    return;
  }

  if (remaining_play_throughs_) {
    encoded_data_next_pos_to_decode_ = 0;
    NotifyInitializeDone();
    return;
  }

  SetState(CS_RESET);
  if (!decoder_deleted())
    DeleteDecoder();
}
