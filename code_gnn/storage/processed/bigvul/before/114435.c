void DXVAVideoDecodeAccelerator::NotifyInputBufferRead(int input_buffer_id) {
  if (client_)
    client_->NotifyEndOfBitstreamBuffer(input_buffer_id);
}
