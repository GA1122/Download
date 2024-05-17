int SkIcoCodec::onGetScanlines(void* dst, int count, size_t rowBytes) {
 SkASSERT(fCurrScanlineCodec);
 return fCurrScanlineCodec->getScanlines(dst, count, rowBytes);
}
