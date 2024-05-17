void VaapiVideoDecodeAccelerator::SurfaceReady(
    const scoped_refptr<VaapiDecodeSurface>& dec_surface) {
  if (message_loop_ != base::MessageLoop::current()) {
    message_loop_->PostTask(
        FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::SurfaceReady,
                              weak_this_, dec_surface));
    return;
  }

  DCHECK(!awaiting_va_surfaces_recycle_);

  {
    base::AutoLock auto_lock(lock_);
    if (state_ == kResetting || state_ == kDestroying)
      return;
  }

  pending_output_cbs_.push(
      base::Bind(&VaapiVideoDecodeAccelerator::OutputPicture, weak_this_,
                 dec_surface->va_surface(), dec_surface->bitstream_id()));

  TryOutputSurface();
}
