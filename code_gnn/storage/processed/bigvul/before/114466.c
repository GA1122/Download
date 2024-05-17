EglRenderingVDAClient::EglRenderingVDAClient(
    RenderingHelper* rendering_helper,
    int rendering_window_id,
    ClientStateNotification* note,
    const std::string& encoded_data,
    int num_NALUs_per_decode,
    int num_in_flight_decodes,
    int num_play_throughs,
    int reset_after_frame_num,
    int delete_decoder_state,
    int profile)
    : rendering_helper_(rendering_helper),
      rendering_window_id_(rendering_window_id),
      encoded_data_(encoded_data), num_NALUs_per_decode_(num_NALUs_per_decode),
      num_in_flight_decodes_(num_in_flight_decodes), outstanding_decodes_(0),
      encoded_data_next_pos_to_decode_(0), next_bitstream_buffer_id_(0),
      note_(note),
      remaining_play_throughs_(num_play_throughs),
      reset_after_frame_num_(reset_after_frame_num),
      delete_decoder_state_(delete_decoder_state),
      state_(CS_CREATED),
      num_decoded_frames_(0), num_done_bitstream_buffers_(0),
      profile_(profile) {
  CHECK_GT(num_NALUs_per_decode, 0);
  CHECK_GT(num_in_flight_decodes, 0);
  CHECK_GT(num_play_throughs, 0);
}
