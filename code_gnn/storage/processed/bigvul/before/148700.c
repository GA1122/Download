void V4L2JpegEncodeAccelerator::Encode(
    scoped_refptr<media::VideoFrame> video_frame,
    int quality,
    BitstreamBuffer* exif_buffer,
    BitstreamBuffer output_buffer) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  DVLOGF(4) << "task_id=" << output_buffer.id()
            << ", size=" << output_buffer.size();

  if (quality <= 0 || quality > 100) {
    VLOGF(1) << "quality is not in range. " << quality;
    NotifyError(output_buffer.id(), INVALID_ARGUMENT);
    return;
  }

  if (video_frame->format() != VideoPixelFormat::PIXEL_FORMAT_I420) {
    VLOGF(1) << "Format is not I420";
    NotifyError(output_buffer.id(), INVALID_ARGUMENT);
    return;
  }

  if (exif_buffer) {
    VLOGF(4) << "EXIF size " << exif_buffer->size();
    if (exif_buffer->size() > kMaxMarkerSizeAllowed) {
      NotifyError(output_buffer.id(), INVALID_ARGUMENT);
      return;
    }
  }

  std::unique_ptr<JobRecord> job_record(new JobRecord(
      video_frame, quality, exif_buffer, std::move(output_buffer)));

  encoder_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&V4L2JpegEncodeAccelerator::EncodeTaskLegacy,
                     base::Unretained(this), base::Passed(&job_record)));
}
