status_t SampleIterator::getSampleSizeDirect(
 uint32_t sampleIndex, size_t *size) {
 *size = 0;

 if (sampleIndex >= mTable->mNumSampleSizes) {
 return ERROR_OUT_OF_RANGE;
 }

 if (mTable->mDefaultSampleSize > 0) {
 *size = mTable->mDefaultSampleSize;
 return OK;
 }

 switch (mTable->mSampleSizeFieldSize) {
 case 32:
 {
 if (mTable->mDataSource->readAt(
                        mTable->mSampleSizeOffset + 12 + 4 * sampleIndex,
                        size, sizeof(*size)) < (ssize_t)sizeof(*size)) {
 return ERROR_IO;
 }

 *size = ntohl(*size);
 break;
 }

 case 16:
 {
 uint16_t x;
 if (mTable->mDataSource->readAt(
                        mTable->mSampleSizeOffset + 12 + 2 * sampleIndex,
 &x, sizeof(x)) < (ssize_t)sizeof(x)) {
 return ERROR_IO;
 }

 *size = ntohs(x);
 break;
 }

 case 8:
 {
 uint8_t x;
 if (mTable->mDataSource->readAt(
                        mTable->mSampleSizeOffset + 12 + sampleIndex,
 &x, sizeof(x)) < (ssize_t)sizeof(x)) {
 return ERROR_IO;
 }

 *size = x;
 break;
 }

 default:
 {
            CHECK_EQ(mTable->mSampleSizeFieldSize, 4);

 uint8_t x;
 if (mTable->mDataSource->readAt(
                        mTable->mSampleSizeOffset + 12 + sampleIndex / 2,
 &x, sizeof(x)) < (ssize_t)sizeof(x)) {
 return ERROR_IO;
 }

 *size = (sampleIndex & 1) ? x & 0x0f : x >> 4;
 break;
 }
 }

 return OK;
}
