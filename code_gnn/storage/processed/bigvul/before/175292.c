grabPartialWakeLock() {
 if (s_callbacks.version >= 13) {
 int ret;
        ret = pthread_mutex_lock(&s_wakeLockCountMutex);
        assert(ret == 0);
        acquire_wake_lock(PARTIAL_WAKE_LOCK, ANDROID_WAKE_LOCK_NAME);

 UserCallbackInfo *p_info =
                internalRequestTimedCallback(wakeTimeoutCallback, NULL, &TIMEVAL_WAKE_TIMEOUT);
 if (p_info == NULL) {
            release_wake_lock(ANDROID_WAKE_LOCK_NAME);
 } else {
            s_wakelock_count++;
 if (s_last_wake_timeout_info != NULL) {
                s_last_wake_timeout_info->userParam = (void *)1;
 }
            s_last_wake_timeout_info = p_info;
 }
        ret = pthread_mutex_unlock(&s_wakeLockCountMutex);
        assert(ret == 0);
 } else {
        acquire_wake_lock(PARTIAL_WAKE_LOCK, ANDROID_WAKE_LOCK_NAME);
 }
}
