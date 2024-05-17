status_t NuMediaExtractor::getSampleTime(int64_t *sampleTimeUs) {
 Mutex::Autolock autoLock(mLock);

 ssize_t minIndex = fetchTrackSamples();

 if (minIndex < 0) {
 return ERROR_END_OF_STREAM;
 }

 TrackInfo *info = &mSelectedTracks.editItemAt(minIndex);
 *sampleTimeUs = info->mSampleTimeUs;

 return OK;
}
