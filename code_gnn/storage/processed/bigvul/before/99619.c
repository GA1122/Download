void VaapiVideoDecodeAccelerator::AssignPictureBuffers(
    const std::vector<media::PictureBuffer>& buffers) {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  base::AutoLock auto_lock(lock_);
  DCHECK(pictures_.empty());

  while (!output_buffers_.empty())
    output_buffers_.pop();

  RETURN_AND_NOTIFY_ON_FAILURE(
      buffers.size() >= requested_num_pics_,
      "Got an invalid number of picture buffers. (Got " << buffers.size()
      << ", requested " << requested_num_pics_ << ")", INVALID_ARGUMENT, );
  DCHECK(requested_pic_size_ == buffers[0].size());

  std::vector<VASurfaceID> va_surface_ids;
  RETURN_AND_NOTIFY_ON_FAILURE(
      vaapi_wrapper_->CreateSurfaces(VA_RT_FORMAT_YUV420, requested_pic_size_,
                                     buffers.size(), &va_surface_ids),
      "Failed creating VA Surfaces", PLATFORM_FAILURE, );
  DCHECK_EQ(va_surface_ids.size(), buffers.size());

  for (size_t i = 0; i < buffers.size(); ++i) {
    DVLOG(2) << "Assigning picture id: " << buffers[i].id()
             << " to texture id: " << buffers[i].texture_id()
             << " VASurfaceID: " << va_surface_ids[i];

    linked_ptr<VaapiPicture> picture(VaapiPicture::CreatePicture(
        vaapi_wrapper_.get(), make_context_current_, buffers[i].id(),
        buffers[i].texture_id(), requested_pic_size_));

    scoped_refptr<gfx::GLImage> image = picture->GetImageToBind();
    if (image) {
      bind_image_.Run(buffers[i].internal_texture_id(),
                      VaapiPicture::GetGLTextureTarget(), image);
    }

    RETURN_AND_NOTIFY_ON_FAILURE(
        picture.get(), "Failed assigning picture buffer to a texture.",
        PLATFORM_FAILURE, );

    bool inserted =
        pictures_.insert(std::make_pair(buffers[i].id(), picture)).second;
    DCHECK(inserted);

    output_buffers_.push(buffers[i].id());
    available_va_surfaces_.push_back(va_surface_ids[i]);
    surfaces_available_.Signal();
  }

  state_ = kDecoding;
  decoder_thread_task_runner_->PostTask(
      FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                            base::Unretained(this)));
}
