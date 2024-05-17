status_t MediaPlayerService::AudioOutput::setParameters(const String8& keyValuePairs)
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 return mTrack->setParameters(keyValuePairs);
}
