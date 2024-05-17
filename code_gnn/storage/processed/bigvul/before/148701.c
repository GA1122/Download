void V4L2JpegEncodeAccelerator::EncodeTask(
    std::unique_ptr<JobRecord> job_record) {
  DCHECK(encoder_task_runner_->BelongsToCurrentThread());
  if (job_record->exif_shm &&
      !job_record->exif_shm->MapAt(job_record->exif_offset,
                                   job_record->exif_shm->size())) {
    VPLOGF(1) << "could not map exif bitstream_buffer";
    NotifyError(job_record->task_id, PLATFORM_FAILURE);
    return;
  }

  gfx::Size coded_size = job_record->input_frame->coded_size();
  if (latest_input_buffer_coded_size_ != coded_size ||
      latest_quality_ != job_record->quality) {
    std::unique_ptr<EncodedInstanceDmaBuf> encoded_device(
        new EncodedInstanceDmaBuf(this));
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

    if (!encoded_device->CreateBuffers(
            coded_size, job_record->input_frame->layout(),
            job_record->output_frame->layout().planes()[0].size)) {
      VLOGF(1) << "Create buffers failed.";
      NotifyError(job_record->task_id, PLATFORM_FAILURE);
      return;
    }

    latest_input_buffer_coded_size_ = coded_size;
    latest_quality_ = job_record->quality;

    encoded_instances_dma_buf_.push(std::move(encoded_device));
  }

  encoded_instances_dma_buf_.back()->input_job_queue_.push(
      std::move(job_record));

  ServiceDeviceTask();
}
