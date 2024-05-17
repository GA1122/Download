ssize_t MediaPlayerService::AudioOutput::bufferSize() const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 return mTrack->frameCount() * mFrameSize;
}
