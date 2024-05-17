SkCodec::Result SkIcoCodec::onGetPixels(const SkImageInfo& dstInfo,
 void* dst, size_t dstRowBytes,
 const Options& opts,
 int* rowsDecoded) {
 if (opts.fSubset) {
 return kUnimplemented;
 }

 int index = 0;
 SkCodec::Result result = kInvalidScale;
 while (true) {
        index = this->chooseCodec(dstInfo.dimensions(), index);
 if (index < 0) {
 break;
 }

 SkCodec* embeddedCodec = fEmbeddedCodecs->operator[](index).get();
        result = embeddedCodec->getPixels(dstInfo, dst, dstRowBytes, &opts);
 switch (result) {
 case kSuccess:
 case kIncompleteInput:
 *rowsDecoded = dstInfo.height();
 return result;
 default:
 break;
 }

        index++;
 }

 SkCodecPrintf("Error: No matching candidate image in ico.\n");
 return result;
}
