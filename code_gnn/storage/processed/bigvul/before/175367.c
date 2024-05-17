ssize_t MyOggExtractor::readPage(off64_t offset, Page *page) {
 uint8_t header[27];
 ssize_t n;
 if ((n = mSource->readAt(offset, header, sizeof(header)))
 < (ssize_t)sizeof(header)) {
        ALOGV("failed to read %zu bytes at offset %#016llx, got %zd bytes",
 sizeof(header), (long long)offset, n);

 if (n < 0) {
 return n;
 } else if (n == 0) {
 return ERROR_END_OF_STREAM;
 } else {
 return ERROR_IO;
 }
 }

 if (memcmp(header, "OggS", 4)) {
 return ERROR_MALFORMED;
 }

 if (header[4] != 0) {

 return ERROR_UNSUPPORTED;
 }

    page->mFlags = header[5];

 if (page->mFlags & ~7) {
 return ERROR_MALFORMED;
 }

    page->mGranulePosition = U64LE_AT(&header[6]);

#if 0
    printf("granulePosition = %llu (0x%llx)\n",
           page->mGranulePosition, page->mGranulePosition);
#endif

    page->mSerialNo = U32LE_AT(&header[14]);
    page->mPageNo = U32LE_AT(&header[18]);

    page->mNumSegments = header[26];
 if (mSource->readAt(
                offset + sizeof(header), page->mLace, page->mNumSegments)
 < (ssize_t)page->mNumSegments) {
 return ERROR_IO;
 }

 size_t totalSize = 0;;
 for (size_t i = 0; i < page->mNumSegments; ++i) {
        totalSize += page->mLace[i];
 }

#if 0
 String8 tmp;
 for (size_t i = 0; i < page->mNumSegments; ++i) {
 char x[32];
        sprintf(x, "%s%u", i > 0 ? ", " : "", (unsigned)page->mLace[i]);

        tmp.append(x);
 }

    ALOGV("%c %s", page->mFlags & 1 ? '+' : ' ', tmp.string());
#endif

 return sizeof(header) + page->mNumSegments + totalSize;
}
