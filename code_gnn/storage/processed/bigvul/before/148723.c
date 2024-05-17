size_t V4L2JpegEncodeAccelerator::EncodedInstance::InputBufferQueuedCount() {
  return input_buffer_map_.size() - free_input_buffers_.size();
}
