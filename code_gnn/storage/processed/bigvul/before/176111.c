SkCodec::Result SkIcoCodec::onStartScanlineDecode(const SkImageInfo& dstInfo,
 const SkCodec::Options& options) {
 int index = 0;
 SkCodec::Result result = kInvalidScale;
 while (true) {
        index = this->chooseCodec(dstInfo.dimensions(), index);
 if (index < 0) {
 break;
 }

 SkCodec* embeddedCodec = fEmbeddedCodecs->operator[](index).get();
        result = embeddedCodec->startScanlineDecode(dstInfo, &options);
 if (kSuccess == result) {
            fCurrScanlineCodec = embeddedCodec;
            fCurrIncrementalCodec = nullptr;
 return result;
 }

        index++;
 }

 SkCodecPrintf("Error: No matching candidate image in ico.\n");
 return result;
}
