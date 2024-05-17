status_t NuMediaExtractor::advance() {
 Mutex::Autolock autoLock(mLock);

 ssize_t minIndex = fetchTrackSamples();

 if (minIndex < 0) {
 return ERROR_END_OF_STREAM;
 }

 TrackInfo *info = &mSelectedTracks.editItemAt(minIndex);

    info->mSample->release();
    info->mSample = NULL;
    info->mSampleTimeUs = -1ll;

 return OK;
}
