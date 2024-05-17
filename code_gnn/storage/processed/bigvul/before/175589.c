ssize_t NuMediaExtractor::fetchTrackSamples(
 int64_t seekTimeUs, MediaSource::ReadOptions::SeekMode mode) {
 TrackInfo *minInfo = NULL;
 ssize_t minIndex = -1;

 for (size_t i = 0; i < mSelectedTracks.size(); ++i) {
 TrackInfo *info = &mSelectedTracks.editItemAt(i);

 if (seekTimeUs >= 0ll) {
            info->mFinalResult = OK;

 if (info->mSample != NULL) {
                info->mSample->release();
                info->mSample = NULL;
                info->mSampleTimeUs = -1ll;
 }
 } else if (info->mFinalResult != OK) {
 continue;
 }

 if (info->mSample == NULL) {
 MediaSource::ReadOptions options;
 if (seekTimeUs >= 0ll) {
                options.setSeekTo(seekTimeUs, mode);
 }
 status_t err = info->mSource->read(&info->mSample, &options);

 if (err != OK) {
                CHECK(info->mSample == NULL);

                info->mFinalResult = err;

 if (info->mFinalResult != ERROR_END_OF_STREAM) {
                    ALOGW("read on track %zu failed with error %d",
                          info->mTrackIndex, err);
 }

                info->mSampleTimeUs = -1ll;
 continue;
 } else {
                CHECK(info->mSample != NULL);
                CHECK(info->mSample->meta_data()->findInt64(
                            kKeyTime, &info->mSampleTimeUs));
 }
 }

 if (minInfo == NULL  || info->mSampleTimeUs < minInfo->mSampleTimeUs) {
            minInfo = info;
            minIndex = i;
 }
 }

 return minIndex;
}
