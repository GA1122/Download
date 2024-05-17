status_t SampleTable::findSyncSampleNear(
 uint32_t start_sample_index, uint32_t *sample_index, uint32_t flags) {
 Mutex::Autolock autoLock(mLock);

 *sample_index = 0;

 if (mSyncSampleOffset < 0) {
 *sample_index = start_sample_index;
 return OK;
 }

 if (mNumSyncSamples == 0) {
 *sample_index = 0;
 return OK;
 }

 uint32_t left = 0;
 uint32_t right_plus_one = mNumSyncSamples;
 while (left < right_plus_one) {
 uint32_t center = left + (right_plus_one - left) / 2;
 uint32_t x = mSyncSamples[center];

 if (start_sample_index < x) {
            right_plus_one = center;
 } else if (start_sample_index > x) {
            left = center + 1;
 } else {
 *sample_index = x;
 return OK;
 }
 }

 if (left == mNumSyncSamples) {
 if (flags == kFlagAfter) {
            ALOGE("tried to find a sync frame after the last one: %d", left);
 return ERROR_OUT_OF_RANGE;
 }
        flags = kFlagBefore;
 }
 else if (left == 0) {
 if (flags == kFlagBefore) {
            ALOGE("tried to find a sync frame before the first one: %d", left);

 }
        flags = kFlagAfter;
 }

 switch (flags) {
 case kFlagBefore:
 {
 --left;
 break;
 }
 case kFlagAfter:
 {
 break;
 }
 default:
 {
            CHECK(flags == kFlagClosest);

 status_t err = mSampleIterator->seekTo(start_sample_index);
 if (err != OK) {
 return err;
 }
 uint32_t sample_time = mSampleIterator->getSampleTime();

            err = mSampleIterator->seekTo(mSyncSamples[left]);
 if (err != OK) {
 return err;
 }
 uint32_t upper_time = mSampleIterator->getSampleTime();

            err = mSampleIterator->seekTo(mSyncSamples[left - 1]);
 if (err != OK) {
 return err;
 }
 uint32_t lower_time = mSampleIterator->getSampleTime();

 if (abs_difference(upper_time, sample_time) >
                abs_difference(sample_time, lower_time)) {
 --left;
 }
 break;
 }
 }

 *sample_index = mSyncSamples[left];
 return OK;
}
