status_t NuMediaExtractor::getSampleTrackIndex(size_t *trackIndex) {
 Mutex::Autolock autoLock(mLock);

 ssize_t minIndex = fetchTrackSamples();

 if (minIndex < 0) {
 return ERROR_END_OF_STREAM;
 }

 TrackInfo *info = &mSelectedTracks.editItemAt(minIndex);
 *trackIndex = info->mTrackIndex;

 return OK;
}
