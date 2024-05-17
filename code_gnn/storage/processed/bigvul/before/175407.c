status_t SampleTable::setSampleSizeParams(
 uint32_t type, off64_t data_offset, size_t data_size) {
 if (mSampleSizeOffset >= 0) {
 return ERROR_MALFORMED;
 }

    CHECK(type == kSampleSizeType32 || type == kSampleSizeTypeCompact);

    mSampleSizeOffset = data_offset;

 if (data_size < 12) {
 return ERROR_MALFORMED;
 }

 uint8_t header[12];
 if (mDataSource->readAt(
                data_offset, header, sizeof(header)) < (ssize_t)sizeof(header)) {
 return ERROR_IO;
 }

 if (U32_AT(header) != 0) {
 return ERROR_MALFORMED;
 }

    mDefaultSampleSize = U32_AT(&header[4]);
    mNumSampleSizes = U32_AT(&header[8]);
 if (mNumSampleSizes > (UINT32_MAX - 12) / 16) {
        ALOGE("b/23247055, mNumSampleSizes(%u)", mNumSampleSizes);
 return ERROR_MALFORMED;
 }

 if (type == kSampleSizeType32) {
        mSampleSizeFieldSize = 32;

 if (mDefaultSampleSize != 0) {
 return OK;
 }

 if (data_size < 12 + mNumSampleSizes * 4) {
 return ERROR_MALFORMED;
 }
 } else {
 if ((mDefaultSampleSize & 0xffffff00) != 0) {
 return ERROR_MALFORMED;
 }

        mSampleSizeFieldSize = mDefaultSampleSize & 0xff;
        mDefaultSampleSize = 0;

 if (mSampleSizeFieldSize != 4 && mSampleSizeFieldSize != 8
 && mSampleSizeFieldSize != 16) {
 return ERROR_MALFORMED;
 }

 if (data_size < 12 + (mNumSampleSizes * mSampleSizeFieldSize + 4) / 8) {
 return ERROR_MALFORMED;
 }
 }

 return OK;
}
