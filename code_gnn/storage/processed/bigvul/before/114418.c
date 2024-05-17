void DXVAVideoDecodeAccelerator::ClearState() {
  last_input_buffer_id_ = -1;
  output_picture_buffers_.clear();
  pending_output_samples_.clear();
}
