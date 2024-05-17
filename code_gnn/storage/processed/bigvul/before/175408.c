status_t SampleTable::setSampleToChunkParams(
 off64_t data_offset, size_t data_size) {
 if (mSampleToChunkOffset >= 0) {
 return ERROR_MALFORMED;
 }

    mSampleToChunkOffset = data_offset;

 if (data_size < 8) {
 return ERROR_MALFORMED;
 }

 uint8_t header[8];
 if (mDataSource->readAt(
                data_offset, header, sizeof(header)) < (ssize_t)sizeof(header)) {
 return ERROR_IO;
 }

 if (U32_AT(header) != 0) {
 return ERROR_MALFORMED;
 }

    mNumSampleToChunkOffsets = U32_AT(&header[4]);

 if ((data_size - 8) / sizeof(SampleToChunkEntry) < mNumSampleToChunkOffsets) {
 return ERROR_MALFORMED;
 }

 if ((uint64_t)kMaxTotalSize / sizeof(SampleToChunkEntry) <=
 (uint64_t)mNumSampleToChunkOffsets) {
        ALOGE("Sample-to-chunk table size too large.");
 return ERROR_OUT_OF_RANGE;
 }

    mTotalSize += (uint64_t)mNumSampleToChunkOffsets *
 sizeof(SampleToChunkEntry);
 if (mTotalSize > kMaxTotalSize) {
        ALOGE("Sample-to-chunk table size would make sample table too large.\n"
 "    Requested sample-to-chunk table size = %llu\n"
 "    Eventual sample table size >= %llu\n"
 "    Allowed sample table size = %llu\n",
 (unsigned long long)mNumSampleToChunkOffsets *
 sizeof(SampleToChunkEntry),
 (unsigned long long)mTotalSize,
 (unsigned long long)kMaxTotalSize);
 return ERROR_OUT_OF_RANGE;
 }

    mSampleToChunkEntries =
 new (std::nothrow) SampleToChunkEntry[mNumSampleToChunkOffsets];
 if (!mSampleToChunkEntries) {
        ALOGE("Cannot allocate sample-to-chunk table with %llu entries.",
 (unsigned long long)mNumSampleToChunkOffsets);
 return ERROR_OUT_OF_RANGE;
 }

 if (mNumSampleToChunkOffsets == 0) {
 return OK;
 }

 if ((off64_t)(kMaxOffset - 8 -
 ((mNumSampleToChunkOffsets - 1) * sizeof(SampleToChunkEntry)))
 < mSampleToChunkOffset) {
 return ERROR_MALFORMED;
 }

 for (uint32_t i = 0; i < mNumSampleToChunkOffsets; ++i) {
 uint8_t buffer[sizeof(SampleToChunkEntry)];

 if (mDataSource->readAt(
                    mSampleToChunkOffset + 8 + i * sizeof(SampleToChunkEntry),
                    buffer,
 sizeof(buffer))
 != (ssize_t)sizeof(buffer)) {
 return ERROR_IO;
 }
 if (U32_AT(buffer) < 1) {
            ALOGE("b/23534160");
 return ERROR_OUT_OF_RANGE;
 }

        mSampleToChunkEntries[i].startChunk = U32_AT(buffer) - 1;
        mSampleToChunkEntries[i].samplesPerChunk = U32_AT(&buffer[4]);
        mSampleToChunkEntries[i].chunkDesc = U32_AT(&buffer[8]);
 }

 return OK;
}
