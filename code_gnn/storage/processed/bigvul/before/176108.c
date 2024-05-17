SkCodec::Result SkIcoCodec::onIncrementalDecode(int* rowsDecoded) {
 SkASSERT(fCurrIncrementalCodec);
 return fCurrIncrementalCodec->incrementalDecode(rowsDecoded);
}
