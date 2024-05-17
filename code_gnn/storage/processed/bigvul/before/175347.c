void MyOggExtractor::buildTableOfContents() {
 off64_t offset = mFirstDataOffset;
 Page page;
 ssize_t pageSize;
 while ((pageSize = readPage(offset, &page)) > 0) {
        mTableOfContents.push();

 TOCEntry &entry =
            mTableOfContents.editItemAt(mTableOfContents.size() - 1);

        entry.mPageOffset = offset;
        entry.mTimeUs = getTimeUsOfGranule(page.mGranulePosition);

        offset += (size_t)pageSize;
 }


 static const size_t kMaxTOCSize = 8192;
 static const size_t kMaxNumTOCEntries = kMaxTOCSize / sizeof(TOCEntry);

 size_t numerator = mTableOfContents.size();

 if (numerator > kMaxNumTOCEntries) {
 size_t denom = numerator - kMaxNumTOCEntries;

 size_t accum = 0;
 for (ssize_t i = mTableOfContents.size() - 1; i >= 0; --i) {
            accum += denom;
 if (accum >= numerator) {
                mTableOfContents.removeAt(i);
                accum -= numerator;
 }
 }
 }
}
