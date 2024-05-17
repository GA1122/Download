status_t MyOggExtractor::findNextPage(
 off64_t startOffset, off64_t *pageOffset) {
 *pageOffset = startOffset;

 for (;;) {
 char signature[4];
 ssize_t n = mSource->readAt(*pageOffset, &signature, 4);

 if (n < 4) {
 *pageOffset = 0;

 return (n < 0) ? n : (status_t)ERROR_END_OF_STREAM;
 }

 if (!memcmp(signature, "OggS", 4)) {
 if (*pageOffset > startOffset) {
                ALOGV("skipped %lld bytes of junk to reach next frame",
 (long long)(*pageOffset - startOffset));
 }

 return OK;
 }

 ++*pageOffset;
 }
}
