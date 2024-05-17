status_t QueryCodecs(
 const sp<IOMX> &omx,
 const char *mimeType, bool queryDecoders,
 Vector<CodecCapabilities> *results) {
 return QueryCodecs(omx, mimeType, queryDecoders, false  , results);
}
