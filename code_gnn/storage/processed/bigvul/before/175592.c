status_t NuMediaExtractor::getSampleMeta(sp<MetaData> *sampleMeta) {
 Mutex::Autolock autoLock(mLock);

 *sampleMeta = NULL;

 ssize_t minIndex = fetchTrackSamples();

 if (minIndex < 0) {
 return ERROR_END_OF_STREAM;
 }

 TrackInfo *info = &mSelectedTracks.editItemAt(minIndex);
 *sampleMeta = info->mSample->meta_data();

 return OK;
}
