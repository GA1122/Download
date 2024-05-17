status_t SampleIterator::getChunkOffset(uint32_t chunk, off64_t *offset) {
 *offset = 0;

 if (chunk >= mTable->mNumChunkOffsets) {
 return ERROR_OUT_OF_RANGE;
 }

 if (mTable->mChunkOffsetType == SampleTable::kChunkOffsetType32) {
 uint32_t offset32;

 if (mTable->mDataSource->readAt(
                    mTable->mChunkOffsetOffset + 8 + 4 * chunk,
 &offset32,
 sizeof(offset32)) < (ssize_t)sizeof(offset32)) {
 return ERROR_IO;
 }

 *offset = ntohl(offset32);
 } else {
        CHECK_EQ(mTable->mChunkOffsetType, SampleTable::kChunkOffsetType64);

 uint64_t offset64;
 if (mTable->mDataSource->readAt(
                    mTable->mChunkOffsetOffset + 8 + 8 * chunk,
 &offset64,
 sizeof(offset64)) < (ssize_t)sizeof(offset64)) {
 return ERROR_IO;
 }

 *offset = ntoh64(offset64);
 }

 return OK;
}
