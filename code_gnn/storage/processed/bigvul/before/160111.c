BackendImpl::~BackendImpl() {
  if (user_flags_ & kNoRandom) {
    background_queue_.WaitForPendingIO();
  } else {
    background_queue_.DropPendingIO();
  }

  if (background_queue_.BackgroundIsCurrentSequence()) {
    CleanupCache();
  } else {
    background_queue_.background_thread()->PostTask(
        FROM_HERE, base::Bind(&FinalCleanupCallback, base::Unretained(this)));
    base::ThreadRestrictions::ScopedAllowWait allow_wait;
    done_.Wait();
  }
}
