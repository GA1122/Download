uint32_t MediaPlayerService::AudioOutput::getSampleRate() const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return 0;
 return mTrack->getSampleRate();
}
