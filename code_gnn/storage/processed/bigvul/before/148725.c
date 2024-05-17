V4L2JpegEncodeAccelerator::JobRecord::JobRecord(
    scoped_refptr<VideoFrame> input_frame,
    int quality,
    BitstreamBuffer* exif_buffer,
    BitstreamBuffer output_buffer)
    : input_frame(input_frame),
      quality(quality),
      task_id(output_buffer.id()),
      output_shm(output_buffer.TakeRegion(), output_buffer.size(), false),
      output_offset(output_buffer.offset()),
      exif_shm(nullptr) {
  if (exif_buffer) {
    exif_shm.reset(new UnalignedSharedMemory(exif_buffer->TakeRegion(),
                                             exif_buffer->size(), false));
    exif_offset = exif_buffer->offset();
  }
}
