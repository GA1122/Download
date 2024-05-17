releaseWakeLock() {
 if (s_callbacks.version >= 13) {
 int ret;
        ret = pthread_mutex_lock(&s_wakeLockCountMutex);
        assert(ret == 0);

 if (s_wakelock_count > 1) {
            s_wakelock_count--;
 } else {
            s_wakelock_count = 0;
            release_wake_lock(ANDROID_WAKE_LOCK_NAME);
 if (s_last_wake_timeout_info != NULL) {
                s_last_wake_timeout_info->userParam = (void *)1;
 }
 }

        ret = pthread_mutex_unlock(&s_wakeLockCountMutex);
        assert(ret == 0);
 } else {
        release_wake_lock(ANDROID_WAKE_LOCK_NAME);
 }
}
