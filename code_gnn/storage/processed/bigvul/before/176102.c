SkSampler* SkIcoCodec::getSampler(bool createIfNecessary) {
 if (fCurrScanlineCodec) {
 SkASSERT(!fCurrIncrementalCodec);
 return fCurrScanlineCodec->getSampler(createIfNecessary);
 }

 if (fCurrIncrementalCodec) {
 return fCurrIncrementalCodec->getSampler(createIfNecessary);
 }

 return nullptr;
}
