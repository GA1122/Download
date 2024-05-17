bool VaapiVideoDecodeAccelerator::WaitForSurfaces_Locked() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  lock_.AssertAcquired();

  while (available_va_surfaces_.empty() &&
         (state_ == kDecoding || state_ == kIdle)) {
    surfaces_available_.Wait();
  }

  return state_ == kDecoding || state_ == kIdle;
}
