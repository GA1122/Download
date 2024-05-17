uint32_t SparseBitSet::nextSetBit(uint32_t fromIndex) const {
 if (fromIndex >= mMaxVal) {
 return kNotFound;
 }
 uint32_t fromPage = fromIndex >> kLogValuesPerPage;
 const element* bitmap = &mBitmaps[mIndices[fromPage]];
 uint32_t offset = (fromIndex & kPageMask) >> kLogBitsPerEl;
    element e = bitmap[offset] & (kElAllOnes >> (fromIndex & kElMask));
 if (e != 0) {
 return (fromIndex & ~kElMask) + CountLeadingZeros(e);
 }
 for (uint32_t j = offset + 1; j < (1 << (kLogValuesPerPage - kLogBitsPerEl)); j++) {
        e = bitmap[j];
 if (e != 0) {
 return (fromIndex & ~kPageMask) + (j << kLogBitsPerEl) + CountLeadingZeros(e);
 }
 }
 uint32_t maxPage = (mMaxVal + kPageMask) >> kLogValuesPerPage;
 for (uint32_t page = fromPage + 1; page < maxPage; page++) {
 uint32_t index = mIndices[page];
 if (index == mZeroPageIndex) {
 continue;
 }
        bitmap = &mBitmaps[index];
 for (uint32_t j = 0; j < (1 << (kLogValuesPerPage - kLogBitsPerEl)); j++) {
            e = bitmap[j];
 if (e != 0) {
 return (page << kLogValuesPerPage) + (j << kLogBitsPerEl) + CountLeadingZeros(e);
 }
 }
 }
 return kNotFound;
}
