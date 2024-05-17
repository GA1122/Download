  void SetResponseHook(ResponseHook response_hook) {
    base::AutoLock auto_lock(lock_);
    response_hook_ = response_hook;
  }
