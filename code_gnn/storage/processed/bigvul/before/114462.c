void EglRenderingVDAClient::DecodeNextNALUs() {
  if (decoder_deleted())
    return;
  if (encoded_data_next_pos_to_decode_ == encoded_data_.size()) {
    if (outstanding_decodes_ == 0) {
      decoder_->Flush();
      SetState(CS_FLUSHING);
    }
    return;
  }
  size_t start_pos = encoded_data_next_pos_to_decode_;
  size_t end_pos;
  GetRangeForNextNALUs(start_pos, &end_pos);

  base::SharedMemory shm;
  CHECK(shm.CreateAndMapAnonymous(end_pos - start_pos))
      << start_pos << ", " << end_pos;
  memcpy(shm.memory(), encoded_data_.data() + start_pos, end_pos - start_pos);
  base::SharedMemoryHandle dup_handle;
  CHECK(shm.ShareToProcess(base::Process::Current().handle(), &dup_handle));
  media::BitstreamBuffer bitstream_buffer(
      next_bitstream_buffer_id_++, dup_handle, end_pos - start_pos);
  decoder_->Decode(bitstream_buffer);
  ++outstanding_decodes_;
  encoded_data_next_pos_to_decode_ = end_pos;

  if (!remaining_play_throughs_ &&
      -delete_decoder_state_ == next_bitstream_buffer_id_) {
    DeleteDecoder();
  }
}
