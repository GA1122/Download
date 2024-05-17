int64_t MediaPlayerService::AudioOutput::getBufferDurationInUs() const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) {
 return 0;
 }
 int64_t duration;
 if (mTrack->getBufferDurationInUs(&duration) != OK) {
 return 0;
 }
 return duration;
}
