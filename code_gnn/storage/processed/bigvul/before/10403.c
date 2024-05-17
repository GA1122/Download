lock_cache (void)
{
  FcMutex *lock;
retry:
  lock = fc_atomic_ptr_get (&cache_lock);
  if (!lock) {
    lock = (FcMutex *) malloc (sizeof (FcMutex));
    FcMutexInit (lock);
    if (!fc_atomic_ptr_cmpexch (&cache_lock, NULL, lock)) {
      FcMutexFinish (lock);
      goto retry;
    }

    FcMutexLock (lock);
     
    FcRandom ();
    return;
  }
  FcMutexLock (lock);
}
