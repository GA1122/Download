bool Camera3Device::tryLockSpinRightRound(Mutex& lock) {
 bool gotLock = false;
 for (size_t i = 0; i < kDumpLockAttempts; ++i) {
 if (lock.tryLock() == NO_ERROR) {
            gotLock = true;
 break;
 } else {
            usleep(kDumpSleepDuration);
 }
 }
 return gotLock;
}
