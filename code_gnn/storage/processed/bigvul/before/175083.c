void SoundPool::quit()
{
    mRestartLock.lock();
    mQuit = true;
    mCondition.signal();
    mCondition.wait(mRestartLock);
    ALOGV("return from quit");
    mRestartLock.unlock();
}
