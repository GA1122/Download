void MojoVideoEncodeAcceleratorService::BitstreamBufferReady(
    int32_t bitstream_buffer_id,
    size_t payload_size,
    bool key_frame,
    base::TimeDelta timestamp) {
  DVLOG(2) << __func__ << " bitstream_buffer_id=" << bitstream_buffer_id
           << ", payload_size=" << payload_size
           << "B,  key_frame=" << key_frame;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!vea_client_)
    return;

  vea_client_->BitstreamBufferReady(bitstream_buffer_id, payload_size,
                                    key_frame, timestamp);
}
