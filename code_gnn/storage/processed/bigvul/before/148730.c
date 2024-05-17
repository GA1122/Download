size_t V4L2JpegEncodeAccelerator::EncodedInstance::OutputBufferQueuedCount() {
  return output_buffer_map_.size() - free_output_buffers_.size();
}
