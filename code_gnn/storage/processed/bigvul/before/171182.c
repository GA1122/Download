float MediaPlayerService::AudioOutput::msecsPerFrame() const
{
 Mutex::Autolock lock(mLock);
 return mMsecsPerFrame;
}
