void VideoEncodeAcceleratorClient::BitstreamBufferReady(
    int32_t bitstream_buffer_id,
    uint32_t payload_size,
    bool key_frame,
    base::TimeDelta timestamp) {
  DVLOG(2) << __func__ << " bitstream_buffer_id=" << bitstream_buffer_id
           << ", payload_size=" << payload_size
           << "B,  key_frame=" << key_frame;
  client_->BitstreamBufferReady(bitstream_buffer_id, payload_size, key_frame,
                                timestamp);
}
