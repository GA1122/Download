bool VaapiVideoDecodeAccelerator::WaitForSurfaces_Locked() {
  lock_.AssertAcquired();
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());

  while (available_va_surfaces_.empty() &&
         (state_ == kDecoding || state_ == kFlushing || state_ == kIdle)) {
    surfaces_available_.Wait();
  }

  if (state_ != kDecoding && state_ != kFlushing && state_ != kIdle)
    return false;

  return true;
}
