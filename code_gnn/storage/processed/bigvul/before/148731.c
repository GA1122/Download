V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::OutputBufferQueuedCount() {
  return kBufferCount - free_output_buffers_.size();
}
