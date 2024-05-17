  void AcceleratedStaticBitmapImage::Transfer() {
  CheckThread();
//   DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    EnsureMailbox(kVerifiedSyncToken, GL_NEAREST);
  detach_thread_at_next_check_ = true;
//   DETACH_FROM_THREAD(thread_checker_);
  }