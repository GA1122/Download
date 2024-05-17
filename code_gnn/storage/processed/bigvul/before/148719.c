V4L2JpegEncodeAccelerator::I420BufferRecord::I420BufferRecord()
    : at_device(false) {
  memset(address, 0, sizeof(address));
  memset(length, 0, sizeof(length));
}
