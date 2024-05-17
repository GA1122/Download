static bool tryLock(Mutex& mutex)
{
 bool locked = false;
 for (int i = 0; i < kDumpLockRetries; ++i) {
 if (mutex.tryLock() == NO_ERROR) {
            locked = true;
 break;
 }
        usleep(kDumpLockSleep);
 }
 return locked;
}
