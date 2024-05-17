VaapiVideoDecodeAccelerator::VaapiVideoDecodeAccelerator(
    const base::Callback<bool(void)>& make_context_current,
    const base::Callback<void(uint32, uint32, scoped_refptr<gfx::GLImage>)>&
        bind_image)
    : make_context_current_(make_context_current),
      state_(kUninitialized),
      input_ready_(&lock_),
      surfaces_available_(&lock_),
      message_loop_(base::MessageLoop::current()),
      decoder_thread_("VaapiDecoderThread"),
      num_frames_at_client_(0),
      num_stream_bufs_at_decoder_(0),
      finish_flush_pending_(false),
      awaiting_va_surfaces_recycle_(false),
      requested_num_pics_(0),
      bind_image_(bind_image),
      weak_this_factory_(this) {
  weak_this_ = weak_this_factory_.GetWeakPtr();
  va_surface_release_cb_ = media::BindToCurrentLoop(
      base::Bind(&VaapiVideoDecodeAccelerator::RecycleVASurfaceID, weak_this_));
}
