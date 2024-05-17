void SoundPool::addToRestartList(SoundChannel* channel)
{
 Mutex::Autolock lock(&mRestartLock);
 if (!mQuit) {
        mRestart.push_back(channel);
        mCondition.signal();
 }
}
