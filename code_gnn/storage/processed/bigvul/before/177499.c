uint32_t SparseBitSet::calcNumPages(const uint32_t* ranges, size_t nRanges) {
 bool haveZeroPage = false;
 uint32_t nonzeroPageEnd = 0;
 uint32_t nPages = 0;
 for (size_t i = 0; i < nRanges; i++) {
 uint32_t start = ranges[i * 2];
 uint32_t end = ranges[i * 2 + 1];
 uint32_t startPage = start >> kLogValuesPerPage;
 uint32_t endPage = (end - 1) >> kLogValuesPerPage;
 if (startPage >= nonzeroPageEnd) {
 if (startPage > nonzeroPageEnd) {
 if (!haveZeroPage) {
                    haveZeroPage = true;
                    nPages++;
 }
 }
            nPages++;
 }
        nPages += endPage - startPage;
        nonzeroPageEnd = endPage + 1;
 }
 return nPages;
}
