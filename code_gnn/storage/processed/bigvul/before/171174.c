status_t MediaPlayerService::AudioOutput::getTimestamp(AudioTimestamp &ts) const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 return mTrack->getTimestamp(ts);
}
