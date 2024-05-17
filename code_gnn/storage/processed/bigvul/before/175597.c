void NuMediaExtractor::releaseTrackSamples() {
 for (size_t i = 0; i < mSelectedTracks.size(); ++i) {
 TrackInfo *info = &mSelectedTracks.editItemAt(i);

 if (info->mSample != NULL) {
            info->mSample->release();
            info->mSample = NULL;

            info->mSampleTimeUs = -1ll;
 }
 }
}
