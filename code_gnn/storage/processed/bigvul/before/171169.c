status_t MediaPlayerService::AudioOutput::getPosition(uint32_t *position) const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 return mTrack->getPosition(position);
}
