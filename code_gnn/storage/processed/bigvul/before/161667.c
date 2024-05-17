void VaapiVideoDecodeAccelerator::AssignPictureBuffers(
    const std::vector<PictureBuffer>& buffers) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  base::AutoLock auto_lock(lock_);
  DCHECK(pictures_.empty());

  while (!output_buffers_.empty())
    output_buffers_.pop();

  RETURN_AND_NOTIFY_ON_FAILURE(
      buffers.size() >= requested_num_pics_,
      "Got an invalid number of picture buffers. (Got " << buffers.size()
      << ", requested " << requested_num_pics_ << ")", INVALID_ARGUMENT, );
  DCHECK(requested_pic_size_ == buffers[0].size());

  const unsigned int va_format = GetVaFormatForVideoCodecProfile(profile_);
  std::vector<VASurfaceID> va_surface_ids;
  RETURN_AND_NOTIFY_ON_FAILURE(
      vaapi_wrapper_->CreateSurfaces(va_format, requested_pic_size_,
                                     buffers.size(), &va_surface_ids),
      "Failed creating VA Surfaces", PLATFORM_FAILURE, );
  DCHECK_EQ(va_surface_ids.size(), buffers.size());

  for (size_t i = 0; i < buffers.size(); ++i) {
    uint32_t client_id = !buffers[i].client_texture_ids().empty()
                             ? buffers[i].client_texture_ids()[0]
                             : 0;
    uint32_t service_id = !buffers[i].service_texture_ids().empty()
                              ? buffers[i].service_texture_ids()[0]
                              : 0;

    DCHECK_EQ(buffers[i].texture_target(),
              vaapi_picture_factory_->GetGLTextureTarget());

    std::unique_ptr<VaapiPicture> picture(vaapi_picture_factory_->Create(
        vaapi_wrapper_, make_context_current_cb_, bind_image_cb_,
        buffers[i].id(), requested_pic_size_, service_id, client_id,
        buffers[i].texture_target()));
    RETURN_AND_NOTIFY_ON_FAILURE(
        picture.get(), "Failed creating a VaapiPicture", PLATFORM_FAILURE, );

    if (output_mode_ == Config::OutputMode::ALLOCATE) {
      RETURN_AND_NOTIFY_ON_FAILURE(
          picture->Allocate(output_format_),
          "Failed to allocate memory for a VaapiPicture", PLATFORM_FAILURE, );
      output_buffers_.push(buffers[i].id());
    }
    bool inserted =
        pictures_.insert(std::make_pair(buffers[i].id(), std::move(picture)))
            .second;
    DCHECK(inserted);

    available_va_surfaces_.push_back(va_surface_ids[i]);
    surfaces_available_.Signal();
  }

  if (state_ == kDecoding) {
    decoder_thread_task_runner_->PostTask(
        FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                              base::Unretained(this)));
  }
}
