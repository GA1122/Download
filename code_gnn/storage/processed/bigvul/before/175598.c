status_t NuMediaExtractor::seekTo(
 int64_t timeUs, MediaSource::ReadOptions::SeekMode mode) {
 Mutex::Autolock autoLock(mLock);

 ssize_t minIndex = fetchTrackSamples(timeUs, mode);

 if (minIndex < 0) {
 return ERROR_END_OF_STREAM;
 }

 return OK;
}
