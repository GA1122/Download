void V4L2JpegEncodeAccelerator::EncodeTaskLegacy(
    std::unique_ptr<JobRecord> job_record) {
  DCHECK(encoder_task_runner_->BelongsToCurrentThread());
  if (!job_record->output_shm.MapAt(job_record->output_offset,
                                    job_record->output_shm.size())) {
    VPLOGF(1) << "could not map I420 bitstream_buffer";
    NotifyError(job_record->task_id, PLATFORM_FAILURE);
    return;
  }
  if (job_record->exif_shm &&
      !job_record->exif_shm->MapAt(job_record->exif_offset,
                                   job_record->exif_shm->size())) {
    VPLOGF(1) << "could not map exif bitstream_buffer";
    NotifyError(job_record->task_id, PLATFORM_FAILURE);
    return;
  }

  gfx::Size coded_size = job_record->input_frame->coded_size();
  if (latest_input_buffer_coded_size_legacy_ != coded_size ||
      latest_quality_legacy_ != job_record->quality) {
    std::unique_ptr<EncodedInstance> encoded_device(new EncodedInstance(this));
    VLOGF(1) << "Open Device for quality " << job_record->quality
             << ", width: " << coded_size.width()
             << ", height: " << coded_size.height();
    if (!encoded_device->Initialize()) {
      VLOGF(1) << "Failed to initialize device";
      NotifyError(job_record->task_id, PLATFORM_FAILURE);
      return;
    }

    if (!encoded_device->SetUpJpegParameters(job_record->quality, coded_size)) {
      VLOGF(1) << "SetUpJpegParameters failed";
      NotifyError(job_record->task_id, PLATFORM_FAILURE);
      return;
    }

    if (!encoded_device->CreateBuffers(coded_size,
                                       job_record->output_shm.size())) {
      VLOGF(1) << "Create buffers failed.";
      NotifyError(job_record->task_id, PLATFORM_FAILURE);
      return;
    }

    latest_input_buffer_coded_size_legacy_ = coded_size;
    latest_quality_legacy_ = job_record->quality;

    encoded_instances_.push(std::move(encoded_device));
  }

  encoded_instances_.back()->input_job_queue_.push(std::move(job_record));

  ServiceDeviceTaskLegacy();
}
