VaapiVideoDecodeAccelerator::VaapiVideoDecodeAccelerator(
    const MakeGLContextCurrentCallback& make_context_current_cb,
    const BindGLImageCallback& bind_image_cb)
    : state_(kUninitialized),
      input_ready_(&lock_),
      vaapi_picture_factory_(new VaapiPictureFactory()),
      surfaces_available_(&lock_),
      task_runner_(base::ThreadTaskRunnerHandle::Get()),
      decoder_thread_("VaapiDecoderThread"),
      num_frames_at_client_(0),
      finish_flush_pending_(false),
      awaiting_va_surfaces_recycle_(false),
      requested_num_pics_(0),
      output_format_(gfx::BufferFormat::BGRX_8888),
      profile_(VIDEO_CODEC_PROFILE_UNKNOWN),
      make_context_current_cb_(make_context_current_cb),
      bind_image_cb_(bind_image_cb),
      weak_this_factory_(this) {
  weak_this_ = weak_this_factory_.GetWeakPtr();
  va_surface_release_cb_ = BindToCurrentLoop(
      base::Bind(&VaapiVideoDecodeAccelerator::RecycleVASurfaceID, weak_this_));
}
