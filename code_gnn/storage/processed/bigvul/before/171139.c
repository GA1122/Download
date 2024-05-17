int Antagonizer::callbackThread(void* user)
{
    ALOGD("Antagonizer started");
 Antagonizer* p = reinterpret_cast<Antagonizer*>(user);
 while (!p->mExit) {
 if (p->mActive) {
            ALOGV("send event");
            p->mCb(p->mClient, 0, 0, 0);
 }
        usleep(interval);
 }
 Mutex::Autolock _l(p->mLock);
    p->mCondition.signal();
    ALOGD("Antagonizer stopped");
 return 0;
}
