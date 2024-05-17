status_t SampleTable::setCompositionTimeToSampleParams(
 off64_t data_offset, size_t data_size) {
    ALOGI("There are reordered frames present.");

 if (mCompositionTimeDeltaEntries != NULL || data_size < 8) {
 return ERROR_MALFORMED;
 }

 uint8_t header[8];
 if (mDataSource->readAt(
                data_offset, header, sizeof(header))
 < (ssize_t)sizeof(header)) {
 return ERROR_IO;
 }

 uint32_t flags = U32_AT(header);
 uint32_t version = flags >> 24;
    flags &= 0xffffff;

 if ((version != 0 && version != 1) || flags != 0) {
 return ERROR_MALFORMED;
 }

 size_t numEntries = U32_AT(&header[4]);

 if (((SIZE_MAX / 8) - 1 < numEntries) || (data_size != (numEntries + 1) * 8)) {
 return ERROR_MALFORMED;
 }

    mNumCompositionTimeDeltaEntries = numEntries;
 uint64_t allocSize = (uint64_t)numEntries * 2 * sizeof(int32_t);
 if (allocSize > kMaxTotalSize) {
        ALOGE("Composition-time-to-sample table size too large.");
 return ERROR_OUT_OF_RANGE;
 }

    mTotalSize += allocSize;
 if (mTotalSize > kMaxTotalSize) {
        ALOGE("Composition-time-to-sample table would make sample table too large.\n"
 "    Requested composition-time-to-sample table size = %llu\n"
 "    Eventual sample table size >= %llu\n"
 "    Allowed sample table size = %llu\n",
 (unsigned long long)allocSize,
 (unsigned long long)mTotalSize,
 (unsigned long long)kMaxTotalSize);
 return ERROR_OUT_OF_RANGE;
 }

    mCompositionTimeDeltaEntries = new (std::nothrow) int32_t[2 * numEntries];
 if (!mCompositionTimeDeltaEntries) {
        ALOGE("Cannot allocate composition-time-to-sample table with %llu "
 "entries.", (unsigned long long)numEntries);
 return ERROR_OUT_OF_RANGE;
 }

 if (mDataSource->readAt(data_offset + 8, mCompositionTimeDeltaEntries,
 (size_t)allocSize) < (ssize_t)allocSize) {
 delete[] mCompositionTimeDeltaEntries;
        mCompositionTimeDeltaEntries = NULL;

 return ERROR_IO;
 }

 for (size_t i = 0; i < 2 * numEntries; ++i) {
        mCompositionTimeDeltaEntries[i] = ntohl(mCompositionTimeDeltaEntries[i]);
 }

    mCompositionDeltaLookup->setEntries(
            mCompositionTimeDeltaEntries, mNumCompositionTimeDeltaEntries);

 return OK;
}
