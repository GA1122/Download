status_t MyOggExtractor::findPrevGranulePosition(
 off64_t pageOffset, uint64_t *granulePos) {
 *granulePos = 0;

 off64_t prevPageOffset = 0;
 off64_t prevGuess = pageOffset;
 for (;;) {
 if (prevGuess >= 5000) {
            prevGuess -= 5000;
 } else {
            prevGuess = 0;
 }

        ALOGV("backing up %lld bytes", (long long)(pageOffset - prevGuess));

 status_t err = findNextPage(prevGuess, &prevPageOffset);
 if (err == ERROR_END_OF_STREAM) {
 continue;
 } else if (err != OK) {
 return err;
 }

 if (prevPageOffset < pageOffset || prevGuess == 0) {
 break;
 }
 }

 if (prevPageOffset == pageOffset) {
 return UNKNOWN_ERROR;
 }

    ALOGV("prevPageOffset at %lld, pageOffset at %lld",
 (long long)prevPageOffset, (long long)pageOffset);

 for (;;) {
 Page prevPage;
 ssize_t n = readPage(prevPageOffset, &prevPage);

 if (n <= 0) {
 return (status_t)n;
 }

        prevPageOffset += n;

 if (prevPageOffset == pageOffset) {
 *granulePos = prevPage.mGranulePosition;
 return OK;
 }
 }
}
