free_lock (void)
{
  FcMutex *lock;
  lock = fc_atomic_ptr_get (&cache_lock);
  if (lock && fc_atomic_ptr_cmpexch (&cache_lock, lock, NULL)) {
    FcMutexFinish (lock);
    free (lock);
  }
}
