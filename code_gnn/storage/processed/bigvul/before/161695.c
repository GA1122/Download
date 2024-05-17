void VaapiVideoDecodeAccelerator::ReusePictureBuffer(
    int32_t picture_buffer_id) {
  VLOGF(4) << "picture id=" << picture_buffer_id;
  DCHECK(task_runner_->BelongsToCurrentThread());
  TRACE_EVENT1("Video Decoder", "VAVDA::ReusePictureBuffer", "Picture id",
               picture_buffer_id);

  if (!PictureById(picture_buffer_id)) {
    VLOGF(3) << "got picture id=" << picture_buffer_id
             << " not in use (anymore?).";
    return;
  }

  --num_frames_at_client_;
  TRACE_COUNTER1("Video Decoder", "Textures at client", num_frames_at_client_);

  output_buffers_.push(picture_buffer_id);
  TryOutputSurface();
}
