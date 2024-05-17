V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::InputBufferQueuedCount() {
  return kBufferCount - free_input_buffers_.size();
}
