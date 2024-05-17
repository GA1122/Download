void VideoEncodeAcceleratorClient::RequireBitstreamBuffers(
    uint32_t input_count,
    const gfx::Size& input_coded_size,
    uint32_t output_buffer_size) {
  DVLOG(2) << __func__ << " input_count= " << input_count
           << " input_coded_size= " << input_coded_size.ToString()
           << " output_buffer_size=" << output_buffer_size;
  client_->RequireBitstreamBuffers(input_count, input_coded_size,
                                   output_buffer_size);
}
