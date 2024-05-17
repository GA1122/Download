  AutoLockOnValidThread(base::Lock& lock, base::ThreadChecker* thread_checker)
      : auto_lock_(lock) {
    DCHECK(!thread_checker || thread_checker->CalledOnValidThread());
  }
