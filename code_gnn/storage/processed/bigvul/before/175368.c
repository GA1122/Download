status_t MyOggExtractor::seekToOffset(off64_t offset) {
 if (mFirstDataOffset >= 0 && offset < mFirstDataOffset) {
        offset = mFirstDataOffset;
 }

 off64_t pageOffset;
 status_t err = findNextPage(offset, &pageOffset);

 if (err != OK) {
 return err;
 }

    findPrevGranulePosition(pageOffset, &mPrevGranulePosition);

    mOffset = pageOffset;

    mCurrentPageSize = 0;
    mFirstPacketInPage = true;
    mCurrentPageSamples = 0;
    mCurrentPage.mNumSegments = 0;
    mCurrentPage.mPrevPacketSize = -1;
    mNextLaceIndex = 0;


 return OK;
}
