int SkIcoCodec::chooseCodec(const SkISize& requestedSize, int startIndex) {
 SkASSERT(startIndex >= 0);

 for (int i = startIndex; i < fEmbeddedCodecs->count(); i++) {
 if (fEmbeddedCodecs->operator[](i)->getInfo().dimensions() == requestedSize) {
 return i;
 }
 }

 return -1;
}
