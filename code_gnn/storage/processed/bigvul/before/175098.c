void SoundChannel::stop()
{
 bool stopped;
 {
 Mutex::Autolock lock(&mLock);
        stopped = doStop_l();
 }

 if (stopped) {
        mSoundPool->done_l(this);
 }
}
