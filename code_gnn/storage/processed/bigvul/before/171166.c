status_t MediaPlayerService::AudioOutput::getPlaybackRate(AudioPlaybackRate *rate)
{
    ALOGV("setPlaybackRate");
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) {
 return NO_INIT;
 }
 *rate = mTrack->getPlaybackRate();
 return NO_ERROR;
}
