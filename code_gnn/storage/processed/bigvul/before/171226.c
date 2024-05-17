ssize_t MediaPlayerService::AudioOutput::write(const void* buffer, size_t size, bool blocking)
{
 Mutex::Autolock lock(mLock);
    LOG_ALWAYS_FATAL_IF(mCallback != NULL, "Don't call write if supplying a callback.");

 if (mTrack != 0) {
 return mTrack->write(buffer, size, blocking);
 }
 return NO_INIT;
}
