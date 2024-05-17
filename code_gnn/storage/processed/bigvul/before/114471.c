void EglRenderingVDAClient::NotifyEndOfBitstreamBuffer(
    int32 bitstream_buffer_id) {
  ++num_done_bitstream_buffers_;
  --outstanding_decodes_;
  DecodeNextNALUs();
}
