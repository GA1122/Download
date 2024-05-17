status_t NuMediaExtractor::unselectTrack(size_t index) {
 Mutex::Autolock autoLock(mLock);

 if (mImpl == NULL) {
 return -EINVAL;
 }

 if (index >= mImpl->countTracks()) {
 return -ERANGE;
 }

 size_t i;
 for (i = 0; i < mSelectedTracks.size(); ++i) {
 TrackInfo *info = &mSelectedTracks.editItemAt(i);

 if (info->mTrackIndex == index) {
 break;
 }
 }

 if (i == mSelectedTracks.size()) {
 return OK;
 }

 TrackInfo *info = &mSelectedTracks.editItemAt(i);

 if (info->mSample != NULL) {
        info->mSample->release();
        info->mSample = NULL;

        info->mSampleTimeUs = -1ll;
 }

    CHECK_EQ((status_t)OK, info->mSource->stop());

    mSelectedTracks.removeAt(i);

 return OK;
}
