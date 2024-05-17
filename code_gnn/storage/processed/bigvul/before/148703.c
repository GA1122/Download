void V4L2JpegEncodeAccelerator::EncodeWithDmaBuf(
    scoped_refptr<VideoFrame> input_frame,
    scoped_refptr<VideoFrame> output_frame,
    int quality,
    int32_t task_id,
    BitstreamBuffer* exif_buffer) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  if (quality <= 0 || quality > 100) {
    VLOGF(1) << "quality is not in range. " << quality;
    NotifyError(task_id, INVALID_ARGUMENT);
    return;
  }

  if (input_frame->format() != VideoPixelFormat::PIXEL_FORMAT_NV12) {
    VLOGF(1) << "Format is not NV12";
    NotifyError(task_id, INVALID_ARGUMENT);
    return;
  }

  if (exif_buffer) {
    VLOGF(4) << "EXIF size " << exif_buffer->size();
    if (exif_buffer->size() > kMaxMarkerSizeAllowed) {
      NotifyError(task_id, INVALID_ARGUMENT);
      return;
    }
  }

  std::unique_ptr<JobRecord> job_record(
      new JobRecord(input_frame, output_frame, quality, task_id, exif_buffer));

  encoder_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&V4L2JpegEncodeAccelerator::EncodeTask,
                     base::Unretained(this), base::Passed(&job_record)));
}
