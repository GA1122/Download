void MediaPlayerService::AudioOutput::pause()
{
    ALOGV("pause");
 Mutex::Autolock lock(mLock);
 if (mTrack != 0) mTrack->pause();
}
