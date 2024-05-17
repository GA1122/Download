status_t MPEG4Source::parseSampleAuxiliaryInformationSizes(off64_t offset, off64_t size) {
    ALOGV("parseSampleAuxiliaryInformationSizes");
 uint8_t version;
 if (mDataSource->readAt(
            offset, &version, sizeof(version))
 < (ssize_t)sizeof(version)) {
 return ERROR_IO;
 }

 if (version != 0) {
 return ERROR_UNSUPPORTED;
 }
    offset++;

 uint32_t flags;
 if (!mDataSource->getUInt24(offset, &flags)) {
 return ERROR_IO;
 }
    offset += 3;

 if (flags & 1) {
 uint32_t tmp;
 if (!mDataSource->getUInt32(offset, &tmp)) {
 return ERROR_MALFORMED;
 }
        mCurrentAuxInfoType = tmp;
        offset += 4;
 if (!mDataSource->getUInt32(offset, &tmp)) {
 return ERROR_MALFORMED;
 }
        mCurrentAuxInfoTypeParameter = tmp;
        offset += 4;
 }

 uint8_t defsize;
 if (mDataSource->readAt(offset, &defsize, 1) != 1) {
 return ERROR_MALFORMED;
 }
    mCurrentDefaultSampleInfoSize = defsize;
    offset++;

 uint32_t smplcnt;
 if (!mDataSource->getUInt32(offset, &smplcnt)) {
 return ERROR_MALFORMED;
 }
    mCurrentSampleInfoCount = smplcnt;
    offset += 4;

 if (mCurrentDefaultSampleInfoSize != 0) {
        ALOGV("@@@@ using default sample info size of %d", mCurrentDefaultSampleInfoSize);
 return OK;
 }
 if (smplcnt > mCurrentSampleInfoAllocSize) {
        mCurrentSampleInfoSizes = (uint8_t*) realloc(mCurrentSampleInfoSizes, smplcnt);
        mCurrentSampleInfoAllocSize = smplcnt;
 }

    mDataSource->readAt(offset, mCurrentSampleInfoSizes, smplcnt);
 return OK;
}
