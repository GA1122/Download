status_t SampleTable::getMetaDataForSample(
 uint32_t sampleIndex,
 off64_t *offset,
 size_t *size,
 uint32_t *compositionTime,
 bool *isSyncSample,
 uint32_t *sampleDuration) {
 Mutex::Autolock autoLock(mLock);

 status_t err;
 if ((err = mSampleIterator->seekTo(sampleIndex)) != OK) {
 return err;
 }

 if (offset) {
 *offset = mSampleIterator->getSampleOffset();
 }

 if (size) {
 *size = mSampleIterator->getSampleSize();
 }

 if (compositionTime) {
 *compositionTime = mSampleIterator->getSampleTime();
 }

 if (isSyncSample) {
 *isSyncSample = false;
 if (mSyncSampleOffset < 0) {
 *isSyncSample = true;
 } else {
 size_t i = (mLastSyncSampleIndex < mNumSyncSamples)
 && (mSyncSamples[mLastSyncSampleIndex] <= sampleIndex)
 ? mLastSyncSampleIndex : 0;

 while (i < mNumSyncSamples && mSyncSamples[i] < sampleIndex) {
 ++i;
 }

 if (i < mNumSyncSamples && mSyncSamples[i] == sampleIndex) {
 *isSyncSample = true;
 }

            mLastSyncSampleIndex = i;
 }
 }

 if (sampleDuration) {
 *sampleDuration = mSampleIterator->getSampleDuration();
 }

 return OK;
}
