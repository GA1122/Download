void MojoVideoEncodeAcceleratorService::RequireBitstreamBuffers(
    unsigned int input_count,
    const gfx::Size& input_coded_size,
    size_t output_buffer_size) {
  DVLOG(2) << __func__ << " input_count=" << input_count
           << " input_coded_size=" << input_coded_size.ToString()
           << " output_buffer_size=" << output_buffer_size;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!vea_client_)
    return;

  output_buffer_size_ = output_buffer_size;
  input_coded_size_ = input_coded_size;

  vea_client_->RequireBitstreamBuffers(input_count, input_coded_size,
                                       output_buffer_size);
}
