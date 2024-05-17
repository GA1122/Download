status_t SampleTable::getMaxSampleSize(size_t *max_size) {
 Mutex::Autolock autoLock(mLock);

 *max_size = 0;

 for (uint32_t i = 0; i < mNumSampleSizes; ++i) {
 size_t sample_size;
 status_t err = getSampleSize_l(i, &sample_size);

 if (err != OK) {
 return err;
 }

 if (sample_size > *max_size) {
 *max_size = sample_size;
 }
 }

 return OK;
}
