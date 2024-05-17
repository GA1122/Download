String8 MediaPlayerService::AudioOutput::getParameters(const String8& keys)
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return String8::empty();
 return mTrack->getParameters(keys);
}
