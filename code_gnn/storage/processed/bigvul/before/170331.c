status_t SampleTable::findThumbnailSample(uint32_t *sample_index) {
 Mutex::Autolock autoLock(mLock);

 if (mSyncSampleOffset < 0) {
 *sample_index = 0;
 return OK;
 }

 uint32_t bestSampleIndex = 0;
 size_t maxSampleSize = 0;

 static const size_t kMaxNumSyncSamplesToScan = 20;


 size_t numSamplesToScan = mNumSyncSamples;
 if (numSamplesToScan > kMaxNumSyncSamplesToScan) {
        numSamplesToScan = kMaxNumSyncSamplesToScan;
 }

 for (size_t i = 0; i < numSamplesToScan; ++i) {
 uint32_t x = mSyncSamples[i];

 size_t sampleSize;
 status_t err = getSampleSize_l(x, &sampleSize);
 if (err != OK) {
 return err;
 }

 if (i == 0 || sampleSize > maxSampleSize) {
            bestSampleIndex = x;
            maxSampleSize = sampleSize;
 }
 }

 *sample_index = bestSampleIndex;

 return OK;
}
