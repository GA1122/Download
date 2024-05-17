V4L2JpegEncodeAccelerator::Initialize(
    chromeos_camera::JpegEncodeAccelerator::Client* client) {
  DCHECK(child_task_runner_->BelongsToCurrentThread());

  std::unique_ptr<EncodedInstanceDmaBuf> encoded_device(
      new EncodedInstanceDmaBuf(this));

  if (!encoded_device->Initialize()) {
    VLOGF(1) << "Failed to initialize device";
    return HW_JPEG_ENCODE_NOT_SUPPORTED;
  }

  if (!encoder_thread_.Start()) {
    VLOGF(1) << "encoder thread failed to start";
    return THREAD_CREATION_FAILED;
  }

  client_ = client;

  encoder_task_runner_ = encoder_thread_.task_runner();

  VLOGF(2) << "V4L2JpegEncodeAccelerator initialized.";
  return ENCODE_OK;
}
